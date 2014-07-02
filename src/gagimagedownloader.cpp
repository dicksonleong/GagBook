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

#include "gagimagedownloader.h"

#include <QtCore/QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/QDesktopServices>
#else
#include <QtCore/QStandardPaths>
#endif
#include <QtGui/QImageReader>
#include <QtNetwork/QNetworkReply>
#include <QDebug>

#include "networkmanager.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
static const QString IMAGE_CACHE_PATH = QDesktopServices::storageLocation(QDesktopServices::CacheLocation)
        + "/gagbook";
#else
static const QString IMAGE_CACHE_PATH = QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
        + "/gagbook";
#endif

void GagImageDownloader::initializeCache()
{
    // create the cache dir if not exists
    QDir imageCacheDir(IMAGE_CACHE_PATH);
    if (!imageCacheDir.exists())
        imageCacheDir.mkpath(".");
}

GagImageDownloader::GagImageDownloader(NetworkManager *networkManager, QObject *parent) :
    QObject(parent), m_networkManager(networkManager), m_downloadGIF(false)
{
}

QList<GagObject> GagImageDownloader::gagList() const
{
    return m_gagList;
}

void GagImageDownloader::setGagList(const QList<GagObject> &gagList)
{
    m_gagList = gagList;
}

bool GagImageDownloader::downloadGIF() const
{
    return m_downloadGIF;
}

void GagImageDownloader::setDownloadGIF(bool downloadGIF)
{
    m_downloadGIF = downloadGIF;
}

void GagImageDownloader::start()
{
    // if there is still downloads ongoing when start() is called
    // there will be big problem since m_gagList will be replaced
    Q_ASSERT(m_replyHash.isEmpty());
    foreach (const GagObject &gag, m_gagList) {
        if (gag.imageUrl().isEmpty() && gag.gifImageUrl().isEmpty())
            continue;

        //        if (m_downloadGIF && !gag.isGIF()) {
        //            qWarning("GagImageDownloader::start(): Not GIF, skip");
        //            continue;
        //        }
        const QUrl downloadImageUrl = gag.isGIF() ? gag.gifImageUrl() : gag.imageUrl();

        QNetworkReply *reply = m_networkManager->createGetRequest(downloadImageUrl, NetworkManager::Image);
        // make sure the QNetworkReply will be destroy when this object is destroyed
        reply->setParent(this);
        m_replyHash.insert(reply, gag);
        connect(reply, SIGNAL(finished()), SLOT(onFinished()));
    }
    m_imagesTotal = m_replyHash.count();
    emit downloadProgress(0, m_imagesTotal);
    if (m_imagesTotal == 0)
        emit finished();
}

void GagImageDownloader::stop()
{
    foreach (QNetworkReply *reply, m_replyHash.keys()) {
        reply->abort();
    }
}

void GagImageDownloader::onFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    Q_ASSERT_X(reply != 0, Q_FUNC_INFO, "Unable to cast sender() to QNetworkReply *");

    if (reply->error() == QNetworkReply::NoError) {
        const QString urlStr = reply->url().toString();
        const QString fileName = IMAGE_CACHE_PATH + "/" + urlStr.mid(urlStr.lastIndexOf("/") + 1);

        QFile image(fileName);
        bool opened = image.open(QIODevice::WriteOnly);
        if (opened) {
            image.write(reply->readAll());
            image.close();

            GagObject gag = m_replyHash.value(reply);
            if (m_downloadGIF) {
                if (gag.imageUrl().isEmpty())
                    gag.setImageUrl(QUrl::fromLocalFile(fileName));
                gag.setGifImageUrl(QUrl::fromLocalFile(fileName));
            } else {
                gag.setImageUrl(QUrl::fromLocalFile(fileName));
            }

            //do some resizing magic to get proper images
            QSize requestedSize(540,QImageReader(&image).size().height());
            QImage image(fileName);
            QImage result;
            if (requestedSize.isValid())
                result = image.scaled(requestedSize, Qt::KeepAspectRatio);
            else
                result = image;

            result.save(fileName);

            //save height in model for laterz
            gag.setImageHeight(requestedSize.height());
        } else {
            qWarning("GagImageDownloader::onFinished(): Unable to open QFile [with fileName = %s] for writing: %s",
                     qPrintable(fileName), qPrintable(image.errorString()));
        }
    } else {
        if (reply->error() != QNetworkReply::OperationCanceledError) {
            qWarning("GagImageDownloader::onFinished(): Network error for [%s]: %s",
                     qPrintable(reply->url().toString()), qPrintable(reply->errorString()));
        }
    }

    m_replyHash.remove(reply);
    reply->deleteLater();

    emit downloadProgress(m_imagesTotal - m_replyHash.count(), m_imagesTotal);
    if (m_replyHash.isEmpty())
        emit finished();
}
