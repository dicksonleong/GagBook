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

#include "infinigagrequest.h"

#include <QtCore/QUrl>
#include <../qt-json/json.h>

#include "networkmanager.h"

#include <QDebug>

// For more information about InfiniGAG API, see <https://github.com/k3min/infinigag>

InfiniGagRequest::InfiniGagRequest(NetworkManager *networkManager, const QString &section, QObject *parent) :
    GagRequest(networkManager, section, parent)
{
}

QNetworkReply *InfiniGagRequest::createRequest(const QString &section, const QString &lastId)
{
    QString requestUrl = QString("http://infinigag.eu01.aws.af.cm/%1/%2")
            .arg(section, (lastId.isEmpty() ? "0" : lastId));

    return networkManager()->createGetRequest(QUrl(requestUrl), NetworkManager::JSON);
}

QList<GagObject> InfiniGagRequest::parseResponse(const QByteArray &response)
{
    bool ok;
    const QVariantList results = QtJson::parse(QString::fromUtf8(response), ok).toMap().value("data").toList();

    Q_ASSERT_X(ok, Q_FUNC_INFO, "Error parsing JSON");
    Q_ASSERT_X(!results.isEmpty(), Q_FUNC_INFO, "Error parsing JSON or JSON is empty");

    QList<GagObject> gagList;
    foreach (const QVariant &gagJson, results) {
        const QVariantMap gagJsonMap = gagJson.toMap();

        GagObject gag;
        gag.setId(gagJsonMap.value("id").toString());
        gag.setUrl(gagJsonMap.value("link").toString());
        gag.setTitle(gagJsonMap.value("caption").toString());

        const QString videoUrl = gagJsonMap.value("media").toMap().value("mp4").toString();
        const QString imageUrl = gagJsonMap.value("images").toMap().value("normal").toString();
        if (!videoUrl.isEmpty()) {
            gag.setIsVideo(true);
            gag.setVideoUrl(videoUrl);
        }
        else if (videoUrl.isEmpty() && imageUrl.endsWith(".gif")) {
            gag.setIsGIF(true);
            gag.setGifImageUrl(imageUrl);
        } else {
            gag.setImageUrl(imageUrl);
        }

        gag.setVotesCount(gagJsonMap.value("votes").toMap().value("count").toInt());

        gagList.append(gag);
    }

    return gagList;
}
