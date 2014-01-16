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
#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebElementCollection>

#include "networkmanager.h"

NineGagRequest::NineGagRequest(NetworkManager *networkManager, GagModel::Section section, QObject *parent) :
    GagRequest(networkManager, section, parent)
{
}

QNetworkReply *NineGagRequest::createRequest(GagModel::Section section, const QString &lastId)
{
    QUrl requestUrl("http://9gag.com/" + getSectionText(section));
    if (!lastId.isEmpty())
        requestUrl.addQueryItem("id", lastId);

    return networkManager()->createGetRequest(requestUrl, NetworkManager::JSON);
}

static QWebElementCollection getEntryItemsFromHtml(const QString &html);
static QWebElementCollection getEntryItemsFromJson(const QString &json);
static QList<GagObject> parseGAG(const QWebElementCollection &entryItems);

QList<GagObject> NineGagRequest::parseResponse(const QByteArray &response)
{
    return parseGAG(getEntryItemsFromJson(QString::fromUtf8(response)));
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

// can not use Qt-Json to parse this JSON because it will cause the order
// of entry-item to be sorted when parsed into a QVariantMap
static QWebElementCollection getEntryItemsFromJson(const QString &json)
{
    QString html = "<html>";

    QRegExp entryItemsRx("<article.+<\\\\/article>");
    entryItemsRx.setMinimal(true);
    int pos = 0;
    while ((pos = entryItemsRx.indexIn(json, pos)) != -1) {
        html += entryItemsRx.cap().remove('\\');
        pos += entryItemsRx.matchedLength();
    }

    html += "</html>";

    return getEntryItemsFromHtml(html);
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

        const QWebElement postContainer = element.findFirst("div.post-container");
        if (!postContainer.findFirst("div.nsfw-post").isNull()) {
            gag.setIsNSFW(true);
        } else if (!postContainer.findFirst("span.play").isNull()) {
            if (!postContainer.findFirst("div.video-post").isNull()) {
                gag.setIsVideo(true);
                gag.setImageUrl(postContainer.findFirst("img.youtube-thumb").attribute("src"));
            } else {
                gag.setIsGIF(true);
                gag.setImageUrl(postContainer.findFirst("img.badge-item-img").attribute("src"));
                gag.setGifImageUrl(postContainer.findFirst("div.badge-animated-container-animated").attribute("data-image"));
            }
        } else {
            gag.setImageUrl(postContainer.findFirst("img.badge-item-img").attribute("src"));
        }

        gagList.append(gag);
    }

    return gagList;
}
