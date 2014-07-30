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

#ifndef GAGIMAGEDOWNLOADER_H
#define GAGIMAGEDOWNLOADER_H

#include <QtCore/QObject>
#include <QtCore/QHash>

#include "gagobject.h"

class NetworkManager;
class QNetworkReply;

/*! Download images for list of GagObject

    Encapsulate (network) requests to download images to local cache for a list
    of GagObject.
 */
class GagImageDownloader : public QObject
{
    Q_OBJECT
public:
    /*! Create the cache directory. Must be call at app startup to make sure the directory
        is created, otherwise the images downloaded may failed to save to the cache. */
    static void initializeCache();

    /*! Constructor. */
    explicit GagImageDownloader(NetworkManager *networkManager, QObject *parent = 0);

    /*! Get the gag list that set with setGagList(). */
    QList<GagObject> gagList() const;

    /*! Set the list of GagObject that their images want to be downloaded.
        GagObject::imageUrl() (or GagObject::gifImageUrl() if downloadGIF() is true) must
        be a valid web URL and will be change to a local URL to the cache image file after
        the image has been downloaded. */
    void setGagList(const QList<GagObject> &gagList);

    /*! Get the value of downloadGIF set with setDownloadGIF(). */
    bool downloadGIF() const;

    /*! If this is set to true, GIF images (GagObject::gifImageUrl()) will be downloaded
        instead of normal images (GagObject::imageUrl()). */
    void setDownloadGIF(bool downloadGIF);

    /*! Start the download request. */
    void start();

    /*! Stop and abort all active download request. */
    void stop();

signals:
    /*! Emit when download progress is changed. */
    void downloadProgress(qint64 downloaded, qint64 total);

    /*! Emit when all images has been downloaded. */
    void finished();

private slots:
    void onFinished();

private:
    NetworkManager *m_networkManager;
    QList<GagObject> m_gagList;
    bool m_downloadGIF;

    QHash<QNetworkReply*, GagObject> m_replyHash;
    int m_imagesTotal;
};

#endif // GAGIMAGEDOWNLOADER_H
