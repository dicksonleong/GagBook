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

NineGagRequest::NineGagRequest(Section section, QNetworkAccessManager *manager, QObject *parent) :
    GagRequest(section, manager, parent)
{
}

QUrl NineGagRequest::contructRequestUrl(Section section, const QString &lastId, int page)
{
    if (lastId.isEmpty()) {
        QString requestUrl = "http://9gag.com/" + getSectionText(section);
        if (page > 0)
            requestUrl += "/" + QString::number(page);
        return QUrl(requestUrl);
    }

    QUrl requestUrl("http://9gag.com/new/json");
    QList< QPair<QString,QString> > query;
    query << qMakePair(QString("list"), getSectionText(section));
    query << qMakePair(QString("id"), lastId);
    requestUrl.setQueryItems(query);

    return requestUrl;
}

static QWebElementCollection getEntryItemsFromHtml(const QString &html);
static QWebElementCollection getEntryItemsFromJson(const QString &json);
static QList<GagObject> parseGAG(const QWebElementCollection &entryItems);

QList<GagObject> NineGagRequest::parseResponse(const QByteArray &response)
{
    const QString responseStr = QString::fromUtf8(response);

    if (responseStr.startsWith('{') && responseStr.endsWith('}')) // JSON
        return parseGAG(getEntryItemsFromJson(responseStr));
    else
        return parseGAG(getEntryItemsFromHtml(responseStr));
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
    return webPage.mainFrame()->findAllElements("li.entry-item");
}

// can not use Qt-Json to parse this JSON because it will cause the order
// of entry-item to be sorted when parsed into a QVariantMap
static QWebElementCollection getEntryItemsFromJson(const QString &json)
{
    QString html = "<html>";

    QRegExp entryItemsRx("\"(<li.+<\\\\/li>)\"");
    entryItemsRx.setMinimal(true);
    int pos = 0;
    while ((pos = entryItemsRx.indexIn(json, pos)) != -1) {
        html += entryItemsRx.cap(1).remove('\\');
        pos += entryItemsRx.matchedLength();
    }

    html += "</html>";

    return getEntryItemsFromHtml(html);
}

static QList<GagObject> parseGAG(const QWebElementCollection &entryItems)
{
    QList<GagObject> gagList;

    foreach (const QWebElement &element, entryItems) {
        GagObject gag;
        gag.setId(element.attribute("gagid"));
        gag.setUrl(element.attribute("data-url"));
        gag.setTitle(element.attribute("data-text"));

        const QWebElementCollection imgCollection = element.findAll("img");
        foreach (const QWebElement &img, imgCollection) {
            if (!img.styleProperty("max-width", QWebElement::InlineStyle).isEmpty()) {
                gag.setImageUrl(img.hasAttribute("large-src") ? img.attribute("large-src")
                                                              : img.attribute("src"));
                break;
            }
            else if (img.attribute("alt") == "NSFW") {
                gag.setImageUrl(img.attribute("src"));
                gag.setIsNSFW(true);
                break;
            }
        }

        const QWebElement loved = element.findFirst("span.loved");
        gag.setVotesCount(loved.attribute("votes").toInt());

        const QWebElement commentSpan = element.findFirst("span.comment");
        gag.setCommentsCount(commentSpan.toPlainText().toInt());

        if (element.findFirst("a.play").isNull() == false)
            gag.setIsVideo(true);

        gagList.append(gag);
    }

    return gagList;
}
