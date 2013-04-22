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
#include <QtCore/QStringList>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebElementCollection>

NineGagRequest::NineGagRequest(Section section, QNetworkAccessManager *manager, QObject *parent) :
    GagRequest(section, manager, parent)
{
    // disable JavaScript and rendering of external object
    m_webPage.settings()->setAttribute(QWebSettings::AutoLoadImages, false);
    m_webPage.settings()->setAttribute(QWebSettings::JavascriptEnabled, false);
    m_webPage.settings()->setAttribute(QWebSettings::PrintElementBackgrounds, false);
    m_webPage.settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, false);
    m_webPage.settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, false);
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

static QList<GagObject> parseGAG(const QWebElementCollection &entryItems)
{
    QList<GagObject> gagList;

    foreach (const QWebElement &element, entryItems) {
        if (!element.hasAttribute("gagid"))
            continue;

        GagObject gag;
        gag.setId(element.attribute("gagid"));
        gag.setUrl(element.attribute("data-url"));
        gag.setTitle(element.attribute("data-text"));

        const QWebElementCollection imgCollection = element.findAll("img");
        foreach (const QWebElement &img, imgCollection) {
            if (img.attribute("alt") == "NSFW") {
                gag.setImageUrl(img.attribute("src"));
                gag.setIsNSFW(true);
                break;
            }
            else if (!img.styleProperty("max-width", QWebElement::InlineStyle).isEmpty()) {
                gag.setImageUrl(img.attribute("src"));
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

static QList<GagObject> parseVoteGAG(const QWebElementCollection &entryItems)
{
    QList<GagObject> gagList;

    foreach (const QWebElement &element, entryItems) {
        if (!element.hasAttribute("gagid"))
            continue;

        GagObject gag;
        gag.setId(element.attribute("gagid"));
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

        gagList.append(gag);
    }

    return gagList;
}

QList<GagObject> NineGagRequest::parseResponse(const QByteArray &response, const Section section)
{
    QString responseStr = QString::fromUtf8(response);

    // Extract the <li> tags using QRegExp
    // For web, it looks like <li class=" entry-item" ... <!--end div.info--> ... </li>
    // For JSON, it looks like <li class=\"  entry-item\" ... <!--end div.info--><\/li>
    QStringList entryItemsString;
    QRegExp entryItemRX("<li\\sclass=\\\\?\"[^\"]*entry-item\\\\?\".+<!--end div.info-->.*<\\\\?/li>",
                        Qt::CaseInsensitive);
    entryItemRX.setMinimal(true);
    int pos = 0;
    while ((pos = entryItemRX.indexIn(responseStr, pos)) != -1) {
        entryItemsString << entryItemRX.cap().remove('\\');
        pos += entryItemRX.matchedLength();
    }

    // prepend and append with html and body tags to make it looks like valid html
    entryItemsString.prepend("<!DOCTYPE html><html><body>");
    entryItemsString.append("</body></html>");

    m_webPage.mainFrame()->setHtml(entryItemsString.join(""));

    const QWebElementCollection entryItems = m_webPage.mainFrame()->findAllElements("li");

    QList<GagObject> parsedGagList;
    switch (section) {
    case Vote: parsedGagList = parseVoteGAG(entryItems); break;
    default: parsedGagList = parseGAG(entryItems); break;
    }

    return parsedGagList;
}
