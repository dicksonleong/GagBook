/*
 * Copyright (c) 2012-2013 Dickson Leong.
 * All rights reserved.
 *
 * This file is part of GagBook.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of GagBook nor the names of its contributors may be
 *       used to endorse or promote products derived from this software without
 *       specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "gagrequest.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QCoreApplication> // for qAddPostRoutine()
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtGui/QDesktopServices>
#include <QtGui/QImageReader>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebElementCollection>

#include "qmlutils.h"

static QByteArray USER_AGENT = "GagBook/" + QByteArray(APP_VERSION);
static QString IMAGE_CACHE_PATH = QDesktopServices::storageLocation(QDesktopServices::CacheLocation)
         + "/gagbook";

static void cacheCleanUp()
{
    QDir imageCacheDir(IMAGE_CACHE_PATH);
    QStringList imageFiles = imageCacheDir.entryList(QDir::Files);
    foreach (const QString &imageFile, imageFiles) {
        QFile::remove(IMAGE_CACHE_PATH + "/" + imageFile);
    }
}

void GagRequest::initializeCache()
{
    // create the cache dir if not exists
    QDir imageCacheDir(IMAGE_CACHE_PATH);
    if (!imageCacheDir.exists())
        imageCacheDir.mkpath(".");

    // clean up all the files in cache dir when app is exiting
    qAddPostRoutine(cacheCleanUp);
}

GagRequest::GagRequest(Section section, QNetworkAccessManager *manager, QObject *parent) :
    QObject(parent), m_section(section), m_netManager(manager), m_lastId(-1), m_page(0), m_reply(0)
{
    // disable JavaScript and rendering of external object
    m_webPage.settings()->setAttribute(QWebSettings::AutoLoadImages, false);
    m_webPage.settings()->setAttribute(QWebSettings::JavascriptEnabled, false);
    m_webPage.settings()->setAttribute(QWebSettings::PrintElementBackgrounds, false);
    m_webPage.settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, false);
    m_webPage.settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, false);
}

GagRequest::~GagRequest()
{
    if (m_reply != 0) {
        m_reply->disconnect();
        m_reply->deleteLater();
        m_reply = 0;
    }
}

void GagRequest::setLastId(int lastId)
{
    Q_ASSERT(m_section == Hot || m_section == Trending || m_section == Vote);
    m_lastId = lastId;
}

void GagRequest::setPage(int page)
{
    Q_ASSERT(m_section == TopDay || m_section == TopWeek || m_section == TopMonth || m_section == TopAll);
    m_page = page;
}

void GagRequest::send()
{
    Q_ASSERT(m_reply == 0);

    QNetworkRequest request;
    request.setRawHeader("User-Agent", USER_AGENT);

    if (m_lastId < 0) {
        QString requestUrl = "http://9gag.com/" + getSectionText(m_section);
        if (m_page > 0)
            requestUrl += "/" + QString::number(m_page);
        request.setUrl(QUrl(requestUrl));
        request.setRawHeader("Accept", "text/html");
    }
    else {
        QUrl requestUrl("http://9gag.com/new/json");
        QList< QPair<QString,QString> > query;
        query << qMakePair(QString("list"), getSectionText(m_section));
        query << qMakePair(QString("id"), QString::number(m_lastId));
        requestUrl.setQueryItems(query);

        request.setUrl(requestUrl);
        request.setRawHeader("Accept", "application/json");
        request.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
    }

    m_reply = m_netManager->get(request);
    connect(m_reply, SIGNAL(finished()), this, SLOT(onFinished()));
}

void GagRequest::onFinished()
{
    QMLUtils::instance()->increaseDataDownloaded(m_reply->size());

    if (m_reply->error()) {
        emit failure(m_reply->errorString());
        m_reply->deleteLater();
        m_reply = 0;
        return;
    }

    QString response = QString::fromUtf8(m_reply->readAll());

    m_reply->deleteLater();
    m_reply = 0;

    // Extract the <li> tags using QRegExp
    // For web, it looks like <li class=" entry-item" ... <!--end div.info--> ... </li>
    // For JSON, it looks like <li class=\"  entry-item\" ... <!--end div.info--><\/li>
    QStringList entryItemsString;
    QRegExp entryItemRX("<li\\sclass=\\\\?\"[^\"]*entry-item\\\\?\".+<!--end div.info-->.*<\\\\?/li>",
                        Qt::CaseInsensitive);
    entryItemRX.setMinimal(true);
    int pos = 0;
    while ((pos = entryItemRX.indexIn(response, pos)) != -1) {
        entryItemsString << entryItemRX.cap().remove('\\');
        pos += entryItemRX.matchedLength();
    }

    // prepend and append with html and body tags to make it looks like valid html
    entryItemsString.prepend("<!DOCTYPE html><html><body>");
    entryItemsString.append("</body></html>");

    m_webPage.mainFrame()->setHtml(entryItemsString.join(""));

    const QWebElementCollection entryItems = m_webPage.mainFrame()->findAllElements("li");
    switch (m_section) {
    case Vote: parseVoteGAG(entryItems); break;
    default: parseGAG(entryItems); break;
    }

    if (parsedGagList.isEmpty())
        emit failure("Unable to parse response :(");
    else
        downloadImages();
}

void GagRequest::parseGAG(const QWebElementCollection &entryItems)
{
    foreach (const QWebElement &element, entryItems) {
        if (!element.hasAttribute("data-url"))
            continue;

        GagObject gag;
        gag.setId(element.attribute("gagid").toInt());
        gag.setUrl(element.attribute("data-url"));
        gag.setTitle(element.attribute("data-text"));

        const QWebElement img = element.findFirst("img");
        gag.setImageUrl(img.attribute("src"));

        if (img.attribute("alt") == "NSFW")
            gag.setIsNSFW(true);

        const QWebElement loved = element.findFirst("span.loved");
        gag.setVotesCount(loved.attribute("votes").toInt());

        const QWebElement commentSpan = element.findFirst("span.comment");
        gag.setCommentsCount(commentSpan.toPlainText().toInt());

        if (element.findFirst("a.play").isNull() == false)
            gag.setIsVideo(true);

        parsedGagList.append(gag);
    }
}

void GagRequest::parseVoteGAG(const QWebElementCollection &entryItems)
{
    foreach (const QWebElement &element, entryItems) {
        if (!element.hasAttribute("data-url"))
            continue;

        GagObject gag;
        gag.setId(element.attribute("gagid").toInt());
        gag.setUrl(element.attribute("data-url"));
        gag.setTitle(element.attribute("data-text"));

        const QWebElementCollection imgCollection = element.findAll("img");
        foreach (const QWebElement &img, imgCollection) {
            if (!img.attribute("src").startsWith("http"))
                continue;

            if (img.hasAttribute("large-src"))
                gag.setImageUrl(img.attribute("large-src"));
            else
                gag.setImageUrl(img.attribute("src"));

            if (img.attribute("alt") == "NSFW")
                gag.setIsNSFW(true);

            break;
        }

        const QWebElement loved = element.findFirst("span.loved");
        gag.setVotesCount(loved.attribute("votes").toInt());

        const QWebElement commentSpan = element.findFirst("span.comment");
        gag.setCommentsCount(commentSpan.toPlainText().toInt());

        if (element.findFirst("a.play").isNull() == false)
            gag.setIsVideo(true);

        parsedGagList.append(gag);
    }
}

void GagRequest::downloadImages()
{
    foreach (const GagObject &gag, parsedGagList) {
        QNetworkRequest request;
        request.setUrl(gag.imageUrl());
        request.setRawHeader("User-Agent", USER_AGENT);
        request.setRawHeader("Accept", "image/*");

        QNetworkReply *reply = m_netManager->get(request);
        m_imageDownloadReplyHash.insert(reply, gag);
        connect(reply, SIGNAL(finished()), SLOT(onImageDownloadFinished()));
    }
}

void GagRequest::onImageDownloadFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    Q_ASSERT_X(reply != 0, Q_FUNC_INFO, "Unable to cast sender() to QNetworkReply *");
    QMLUtils::instance()->increaseDataDownloaded(reply->size());

    const QString urlStr = reply->url().toString();
    const QString fileName = IMAGE_CACHE_PATH + "/" + urlStr.mid(urlStr.lastIndexOf("/") + 1);
    m_imageDownloadReplyHash[reply].setImageUrl(QUrl::fromLocalFile(fileName));

    if (reply->error() == QNetworkReply::NoError) {
        const QString contentType = reply->rawHeader("Content-Type");
        if (!contentType.startsWith("image")) {
            qDebug("GagImageDownloader::onImageDownloadFinished(): Downloaded image doesn't has an image/* content type (%s), "
                   "but still continue anyway", qPrintable(contentType));
        }

        QFile image(fileName);
        bool opened = image.open(QIODevice::WriteOnly);
        if (opened) {
            image.write(reply->readAll());
            image.close();
            m_imageDownloadReplyHash[reply].setImageHeight(QImageReader(&image).size().height());
        } else {
            qDebug("GagImageDownloader::onImageDownloadFinished(): Unable to open QFile (with fileName = %s) for writing: %s",
                   qPrintable(fileName), qPrintable(image.errorString()));
        }
    } else {
        qDebug("GagImageDownloader::onImageDownloadFinished(): Network error: %s", qPrintable(reply->errorString()));
    }

    m_imageDownloadReplyHash.remove(reply);
    if (m_imageDownloadReplyHash.isEmpty())
        emit success(parsedGagList);

    reply->deleteLater();
}

QString GagRequest::getSectionText(Section section)
{
    switch (section) {
    case Hot:
        return "hot";
    case Trending:
        return "trending";
    case Vote:
        return "vote";
    case TopDay:
        return "top/day";
    case TopWeek:
        return "top/week";
    case TopMonth:
        return "top/month";
    case TopAll:
        return "top/all";
    default:
        qWarning("GagRequest::getSectionText(): Invalid section");
        return QString("");
    }
}
