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

#include "gagmanager.h"

#include "gagmodel.h"
#include "ninegagrequest.h"
#include "infinigagrequest.h"
#include "gagimagedownloader.h"
#include "networkmanager.h"
#include "settings.h"

GagManager::GagManager(QObject *parent) :
    QObject(parent), m_request(0), m_imageDownloader(0), m_manualImageDownloader(0),
    m_busy(false), m_progress(0), m_model(0), m_downloadingImageIndex(-1)
{
    GagImageDownloader::initializeCache();
}

void GagManager::refresh(RefreshType refreshType)
{
    Q_ASSERT_X(m_model != 0, Q_FUNC_INFO, "model not set");

    if (m_request != 0) {
        m_request->disconnect();
        m_request->deleteLater();
        m_request = 0;
    }

    if (m_imageDownloader != 0) {
        m_imageDownloader->disconnect();
        m_imageDownloader->deleteLater();
        m_imageDownloader = 0;
    }

    GagRequest::Section selectedSection = static_cast<GagRequest::Section>(Settings::instance()->selectedSection());

    switch (Settings::instance()->source()) {
    case 0: m_request = new NineGagRequest(selectedSection, this); break;
    case 1: m_request = new InfiniGagRequest(selectedSection, this); break;
    default: qCritical("GagManager::refresh(): Invalid source");
    }

    if (refreshType == RefreshAll)
        m_model->clear();
    else if (refreshType == RefreshOlder && !m_model->gagList().isEmpty())
        m_request->setLastId(m_model->gagList().last().id());

    connect(m_request, SIGNAL(success(QList<GagObject>)), this, SLOT(onSuccess(QList<GagObject>)));
    connect(m_request, SIGNAL(failure(QString)), this, SLOT(onFailure(QString)));

    m_request->send();

    m_busy = true;
    emit busyChanged();

    if (m_progress != 0) {
        m_progress = 0;
        emit progressChanged();
    }
}

void GagManager::stopRefresh()
{
    if (m_request != 0) {
        m_request->disconnect();
        m_request->deleteLater();
        m_request = 0;
    }
    if (m_imageDownloader != 0)
        m_imageDownloader->stop();

    if (m_busy != false) {
        m_busy = false;
        emit busyChanged();
    }
}

void GagManager::downloadImage(int index)
{
    if (m_manualImageDownloader != 0) {
        m_manualImageDownloader->disconnect();
        m_manualImageDownloader->deleteLater();
        m_manualImageDownloader = 0;
    }

    QList<GagObject> gags;
    gags.append(m_model->gagList().at(index));
    m_downloadingImageIndex = index;
    emit downloadingImageIndexChanged();

    m_manualImageDownloader = new GagImageDownloader(gags, true, this);
    connect(m_manualImageDownloader, SIGNAL(finished(QList<GagObject>)),
            SLOT(onManualDownloadFinished(QList<GagObject>)));
    m_manualImageDownloader->start();
}

bool GagManager::isBusy() const
{
    return m_busy;
}

qreal GagManager::progress() const
{
    return m_progress;
}

GagModel *GagManager::model() const
{
    return m_model;
}

void GagManager::setModel(GagModel *model)
{
    if (m_model != model) {
        m_model = model;
        emit modelChanged();
    }
}

int GagManager::downloadingImageIndex() const
{
    return m_downloadingImageIndex;
}

static bool downloadGIF()
{
    switch (Settings::instance()->autoDownloadGif()) {
    case 0:
        return true;
    case 1:
        if (NetworkManager::isMobileData()) return false;
        return true;
    case 2:
        return false;
    default:
        return true;
    }
}

void GagManager::onSuccess(const QList<GagObject> &gagList)
{
    m_imageDownloader = new GagImageDownloader(gagList, downloadGIF(), this);
    connect(m_imageDownloader, SIGNAL(finished(QList<GagObject>)), SLOT(onDownloadFinished(QList<GagObject>)));
    connect(m_imageDownloader, SIGNAL(downloadProgress(int,int)), SLOT(onImageDownloadProgress(int,int)));
    m_imageDownloader->start();

    m_request->deleteLater();
    m_request = 0;
}

void GagManager::onFailure(const QString &errorMessage)
{
    emit refreshFailure(errorMessage);
    m_request->deleteLater();
    m_request = 0;
    m_busy = false;
    emit busyChanged();
}

void GagManager::onImageDownloadProgress(int imagesDownloaded, int imagesTotal)
{
    qreal progress;
    if (imagesTotal > 0)
        progress = qreal(imagesDownloaded) / qreal(imagesTotal);
    else
        progress = 1;

    if (m_progress != progress) {
        m_progress = progress;
        emit progressChanged();
    }
}

void GagManager::onDownloadFinished(const QList<GagObject> &gagList)
{
    m_model->append(gagList);
    m_imageDownloader->deleteLater();
    m_imageDownloader = 0;
    m_busy = false;
    emit busyChanged();
}

void GagManager::onManualDownloadFinished(const QList<GagObject> &gagList)
{
    Q_UNUSED(gagList);

    m_model->emitDataChanged(m_downloadingImageIndex);
    m_downloadingImageIndex = -1;
    emit downloadingImageIndexChanged();

    m_manualImageDownloader->deleteLater();
    m_manualImageDownloader = 0;
}
