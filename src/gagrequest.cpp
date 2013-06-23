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

#include "gagrequest.h"

#include <QtCore/QDir>
#include <QtGui/QDesktopServices>
#include <QtGui/QImageReader>
#include <QtNetwork/QNetworkReply>

#include "networkmanager.h"

static QString IMAGE_CACHE_PATH = QDesktopServices::storageLocation(QDesktopServices::CacheLocation)
         + "/gagbook";

void GagRequest::initializeCache()
{
    // create the cache dir if not exists
    QDir imageCacheDir(IMAGE_CACHE_PATH);
    if (!imageCacheDir.exists())
        imageCacheDir.mkpath(".");
}

GagRequest::GagRequest(Section section, QObject *parent) :
    QObject(parent), m_section(section), m_reply(0)
{
}

GagRequest::~GagRequest()
{
    if (m_reply != 0) {
        m_reply->disconnect();
        m_reply->deleteLater();
        m_reply = 0;
    }
}

void GagRequest::setLastId(const QString &lastId)
{
    Q_ASSERT(m_section == Hot || m_section == Trending || m_section == Vote);
    m_lastId = lastId;
}

void GagRequest::send()
{
    Q_ASSERT(m_reply == 0);

    m_reply = createRequest(m_section, m_lastId);
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

    QByteArray response = m_reply->readAll();
    m_reply->deleteLater();
    m_reply = 0;

    m_parsedGagList = parseResponse(response);
    if (m_parsedGagList.isEmpty())
        emit failure("Unable to parse response");
    else
        downloadImages();
}

void GagRequest::downloadImages()
{
    foreach (const GagObject &gag, m_parsedGagList) {
        if (gag.imageUrl().isEmpty())
            continue;

        QNetworkReply *reply = NetworkManager::createGetRequest(gag.imageUrl(), NetworkManager::Image);
        m_imageDownloadReplyHash.insert(reply, gag);
        connect(reply, SIGNAL(finished()), SLOT(onImageDownloadFinished()));
    }
}

void GagRequest::onImageDownloadFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    Q_ASSERT_X(reply != 0, Q_FUNC_INFO, "Unable to cast sender() to QNetworkReply *");

    const QString urlStr = reply->url().toString();
    const QString fileName = IMAGE_CACHE_PATH + "/" + urlStr.mid(urlStr.lastIndexOf("/") + 1);
    m_imageDownloadReplyHash[reply].setImageUrl(QUrl::fromLocalFile(fileName));

    if (reply->error() == QNetworkReply::NoError) {
        const QString contentType = reply->rawHeader("Content-Type");
        if (!contentType.startsWith("image")) {
            qDebug("GagRequest::onImageDownloadFinished(): Downloaded image doesn't has an image/* "
                   "content type [%s], but still continue anyway", qPrintable(contentType));
        }

        QFile image(fileName);
        bool opened = image.open(QIODevice::WriteOnly);
        if (opened) {
            image.write(reply->readAll());
            image.close();
            m_imageDownloadReplyHash[reply].setImageHeight(QImageReader(&image).size().height());
        } else {
            qDebug("GagRequest::onImageDownloadFinished(): Unable to open QFile [with fileName = %s] for writing: %s",
                   qPrintable(fileName), qPrintable(image.errorString()));
        }
    } else {
        qDebug("GagRequest::onImageDownloadFinished(): Network error for [%s]: %s",
               qPrintable(reply->url().toString()), qPrintable(reply->errorString()));
    }

    m_imageDownloadReplyHash.remove(reply);
    if (m_imageDownloadReplyHash.isEmpty())
        emit success(m_parsedGagList);

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
    default:
        qWarning("GagRequest::getSectionText(): Invalid section");
        return QString("");
    }
}
