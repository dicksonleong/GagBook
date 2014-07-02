/*
 * Copyright (c) 2014 Dickson Leong.
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

#include "ninegagrequest.h"

#include <QtCore/QRegExp>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QWebElementCollection>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QUrlQuery>
#endif

#include <QUrl>
#include <QDebug>

#include "networkmanager.h"
#include "qmlutils.h"

NineGagRequest::NineGagRequest(NetworkManager *networkManager, const QString &section, QObject *parent) :
    GagRequest(networkManager, section, parent)
{
}

QNetworkReply *NineGagRequest::createRequest(const QString &section, const QString &lastId)
{
    QUrl requestUrl("http://9gag.com/" + section);

    if (!lastId.isEmpty()) {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        requestUrl.addQueryItem("id", lastId);
#else
        QUrlQuery q;
        q.addQueryItem("id", lastId);
        requestUrl.setQuery(q.query());
#endif
    }

    qDebug() << "getting url: " << requestUrl;

    return networkManager()->createGetRequest(requestUrl, NetworkManager::HTML);
}

static QWebElementCollection getEntryItemsFromHtml(const QString &html);
static QList<GagObject> parseGAG(const QWebElementCollection &entryItems);

QList<GagObject> NineGagRequest::parseResponse(const QByteArray &response)
{
    qDebug() << "ineGagRequest::parseResponse: got response with length: " << response.length();

    return parseGAG(getEntryItemsFromHtml(QString::fromUtf8(response)));
}

static QWebElementCollection getEntryItemsFromHtml(const QString &html)
{
    QWebPage webPage;
    // disable JavaScript and rendering of external object
    webPage.settings()->setAttribute(QWebSettings::AutoLoadImages, false);
    webPage.settings()->setAttribute(QWebSettings::JavascriptEnabled, false);
    webPage.settings()->setAttribute(QWebSettings::PrintElementBackgrounds, false);
    webPage.settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, false);
    webPage.settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, false);

    webPage.mainFrame()->setHtml(html);
    return webPage.mainFrame()->findAllElements("article");
}

static const QRegExp dataScriptImgSrcRegExp("<img.*src=\"(http[^\\s\"]+)\".*\\/>");

static QList<GagObject> parseGAG(const QWebElementCollection &entryItems)
{
    QList<GagObject> gagList;

    foreach (const QWebElement &element, entryItems) {
        GagObject gag;
        gag.setId(element.attribute("data-entry-id"));
        gag.setUrl(element.attribute("data-entry-url"));
        gag.setVotesCount(element.attribute("data-entry-votes").toInt());
        gag.setCommentsCount(element.attribute("data-entry-comments").toInt());
        gag.setTitle(element.findFirst("a").toPlainText().trimmed());
        gag.setIsPartialImage(false);

        const QWebElement postContainer = element.findFirst("div.post-container");

        if (!postContainer.findFirst("div.nsfw-post").isNull()) {
            gag.setIsNSFW(true);
        } else if (!postContainer.findFirst("span.play").isNull()) {
            if (!postContainer.findFirst("div.video-post").isNull()) {
                gag.setIsVideo(true);
                gag.setImageUrl(postContainer.findFirst("img.youtube-thumb").attribute("src"));
            } else {
                gag.setIsGIF(true);
                gag.setImageUrl(postContainer.findFirst("div.badge-animated-container-animated").attribute("data-image"));
                gag.setGifImageUrl(postContainer.findFirst("div.badge-animated-container-animated").attribute("data-image"));
            }
        } else if(!element.findFirst("div.post-container.with-button").isNull()) {
            //not full pic, we'll need to go deeper for the full lenght image
            const QUrl regularImgUrl = QString(postContainer.findFirst("img.badge-item-img").attribute("src"));
            const QUrl imgUrl = QString("%1/photo/%2_700b.jpg").arg(regularImgUrl.toString(QUrl::RemovePath)).arg(gag.id());

            Q_ASSERT(imgUrl.isValid());

            qDebug() << "regularImgUrl: " << regularImgUrl;
            qDebug() << "imgUrl: " << imgUrl;

            gag.setFullImageUrl(imgUrl);
            gag.setImageUrl(regularImgUrl);
            gag.setIsPartialImage(true);
        }

        else {
            gag.setImageUrl(postContainer.findFirst("img.badge-item-img").attribute("src"));
        }

        gagList.append(gag);
    }

    return gagList;
}
