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

#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebElementCollection>

static const QByteArray USER_AGENT = "GagBook/" + QByteArray(APP_VERSION);

GagRequest::GagRequest(QNetworkAccessManager *manager, QObject *parent) :
    QObject(parent), m_netManager(manager), m_reply(0)
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

void GagRequest::send(Section section)
{
    Q_ASSERT(m_reply == 0);

    QNetworkRequest request;
    request.setUrl(QUrl("http://9gag.com/" + getSectionText(section)));
    request.setRawHeader("Accept", "text/html");
    request.setRawHeader("User-Agent", USER_AGENT);

    m_reply = m_netManager->get(request);
    connect(m_reply, SIGNAL(finished()), this, SLOT(onFinished()));
}

void GagRequest::send(Section section, int lastId)
{
    Q_ASSERT(m_reply == 0);

    QNetworkRequest request;

    QUrl requestUrl("http://9gag.com/new/json");
    QList< QPair<QString,QString> > query;
    query << qMakePair(QString("list"), getSectionText(section));
    query << qMakePair(QString("id"), QString::number(lastId));
    requestUrl.setQueryItems(query);

    request.setUrl(requestUrl);
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
    request.setRawHeader("User-Agent", USER_AGENT);

    m_reply = m_netManager->get(request);
    connect(m_reply, SIGNAL(finished()), this, SLOT(onFinished()));
}

void GagRequest::onFinished()
{
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
    QStringList entryItems;
    QRegExp entryItemRX("<li\\sclass=\\\\?\"\\s*entry-item\\\\?\".+<!--end div.info-->.*<\\\\?/li>", Qt::CaseInsensitive);
    entryItemRX.setMinimal(true);
    int pos = 0;
    while ((pos = entryItemRX.indexIn(response, pos)) != -1) {
        entryItems << entryItemRX.cap().remove('\\');
        pos += entryItemRX.matchedLength();
    }

    // prepend and append with html and body tags to make it looks like valid html
    entryItems.prepend("<!DOCTYPE html><html><body>");
    entryItems.append("</body></html>");

    m_webPage.mainFrame()->setHtml(entryItems.join(""));

    const QWebElementCollection entryItemsCollections = m_webPage.mainFrame()->findAllElements("li");
    foreach (const QWebElement &element, entryItemsCollections) {
        if (!element.hasAttribute("data-url"))
            continue;

        GagObject gag;
        gag.setId(element.attribute("gagid").toInt());
        gag.setUrl(element.attribute("data-url"));
        gag.setTitle(element.attribute("data-text"));

        const QWebElement img = element.findFirst("img");
        gag.setImageUrl(img.attribute("src"));

        const QWebElement loved = element.findFirst("span.loved");
        gag.setVotesCount(loved.attribute("votes").toInt());

        parsedGagList.append(gag);
    }

    if (parsedGagList.isEmpty())
        emit failure("Unable to parse response :(");
    else
        emit success(parsedGagList);
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
    default:
        qWarning("GagRequest::getSectionText(): Invalid section");
        return QString("");
    }
}
