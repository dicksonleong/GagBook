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

#include "gagimagedownloader.h"

#include <QtCore/QDir>
#include <QtGui/QDesktopServices>
#include <QtGui/QImageReader>
#include <QtNetwork/QNetworkReply>

#include "networkmanager.h"

static const QString IMAGE_CACHE_PATH = QDesktopServices::storageLocation(QDesktopServices::CacheLocation)
         + "/gagbook";

void GagImageDownloader::initializeCache()
{
    // create the cache dir if not exists
    QDir imageCacheDir(IMAGE_CACHE_PATH);
    if (!imageCacheDir.exists())
        imageCacheDir.mkpath(".");
}

GagImageDownloader::GagImageDownloader(const QList<GagObject> &gagList, bool downloadGIF, QObject *parent) :
    QObject(parent), m_gagList(gagList), m_downloadGIF(downloadGIF)
{
}

void GagImageDownloader::start()
{
    foreach (const GagObject &gag, m_gagList) {
        if (gag.imageUrl().isEmpty() || (!m_downloadGIF && gag.isGIF()))
            continue;

        QNetworkReply *reply = NetworkManager::createGetRequest(gag.imageUrl(), NetworkManager::Image);
        // make sure the QNetworkReply will be destroy when this object is destroyed
        reply->setParent(this);
        m_replyHash.insert(reply, gag);
        connect(reply, SIGNAL(finished()), SLOT(onFinished()));
    }
    m_imagesTotal = m_replyHash.count();
    emit downloadProgress(0, m_imagesTotal);
}

void GagImageDownloader::onFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    Q_ASSERT_X(reply != 0, Q_FUNC_INFO, "Unable to cast sender() to QNetworkReply *");

    const QString urlStr = reply->url().toString();
    const QString fileName = IMAGE_CACHE_PATH + "/" + urlStr.mid(urlStr.lastIndexOf("/") + 1);

    if (reply->error() == QNetworkReply::NoError) {
        QFile image(fileName);
        bool opened = image.open(QIODevice::WriteOnly);
        if (opened) {
            image.write(reply->readAll());
            image.close();
            m_replyHash[reply].setImageUrl(QUrl::fromLocalFile(fileName));
            m_replyHash[reply].setImageHeight(QImageReader(&image).size().height());
        } else {
            qDebug("GagImageDownloader::onFinished(): Unable to open QFile [with fileName = %s] for writing: %s",
                   qPrintable(fileName), qPrintable(image.errorString()));
        }
    } else {
        qDebug("GagImageDownloader::onFinished(): Network error for [%s]: %s",
               qPrintable(reply->url().toString()), qPrintable(reply->errorString()));
    }

    m_replyHash.remove(reply);
    reply->deleteLater();

    emit downloadProgress(m_imagesTotal - m_replyHash.count(), m_imagesTotal);
    if (m_replyHash.isEmpty())
        emit finished(m_gagList);
}
