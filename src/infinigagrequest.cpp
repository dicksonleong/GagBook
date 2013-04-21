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

#include "infinigagrequest.h"

#include <QtCore/QUrl>
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"
#include "json/qjsonarray.h"

InfiniGagRequest::InfiniGagRequest(Section section, QNetworkAccessManager *manager, QObject *parent) :
    GagRequest(section, manager, parent)
{
}

QUrl InfiniGagRequest::contructRequestUrl(Section section, const QString &lastId, int page)
{
    Q_UNUSED(page)

    QString requestUrl = QString("http://infinigag.eu01.aws.af.cm/2/%1/%2.json")
            .arg(getSectionText(section), (lastId.isEmpty() ? "0" : lastId));

    return QUrl(requestUrl);
}

QList<GagObject> InfiniGagRequest::parseResponse(const QByteArray &response, const Section section)
{
    Q_UNUSED(section)

    QJsonParseError parseError;
    const QJsonDocument jsonDocument = QJsonDocument::fromJson(response, &parseError);

    Q_ASSERT_X(parseError.error == QJsonParseError::NoError, Q_FUNC_INFO, "Error parsing JSON");

    const QJsonArray gagJsonArray = jsonDocument.object().value("data").toArray();

    QList<GagObject> gagList;
    foreach (const QJsonValue &gagJsonValue, gagJsonArray) {
        const QJsonObject gagJsonObject = gagJsonValue.toObject();

        GagObject gag;
        gag.setId(gagJsonObject.value("id").toString());
        // the "links" from InfiniGAG is wrong, so I contruct the url myself
        gag.setUrl("http://9gag.com/gag/" + gag.id());
        gag.setTitle(gagJsonObject.value("caption").toString());
        gag.setImageUrl(gagJsonObject.value("images").toObject().value("normal").toString());
        gag.setVotesCount(gagJsonObject.value("votes").toObject().value("count").toDouble());

        gagList.append(gag);
    }

    return gagList;
}
