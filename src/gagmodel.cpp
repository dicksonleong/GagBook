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

#include "gagmodel.h"

#include <QtCore/QUrl>
#include <QDebug>

#include "gagbookmanager.h"
#include "appsettings.h"
#include "networkmanager.h"
#include "infinigagrequest.h"
#include "ninegagrequest.h"
#include "gagimagedownloader.h"

GagModel::GagModel(QObject *parent) :
    QAbstractListModel(parent), m_busy(false), m_progress(0), m_manager(0), m_selectedSection(0),
    m_request(0), m_imageDownloader(0), m_manualImageDownloader(0), m_downloadingIndex(-1)
{
    _roles[TitleRole] = "title";
    _roles[UrlRole] = "url";
    _roles[ImageUrlRole] = "imageUrl";
    _roles[FullImageUrlRole] = "fullImageUrl";
    _roles[GifImageUrlRole] = "gifImageUrl";
    _roles[ImageHeightRole] = "imageHeight";
    _roles[VotesCountRole] = "votesCount";
    _roles[CommentsCountRole] = "commentsCount";
    _roles[IsVideoRole] = "isVideo";
    _roles[IsNSFWRole] = "isNSFW";
    _roles[IsGIFRole] = "isGIF";
    _roles[IsPartialImageRole] = "isPartialImage";
    _roles[IsDownloadingRole] = "isDownloading";
}

QHash<int, QByteArray> GagModel::roleNames() const {
  return _roles;
}

void GagModel::classBegin()
{
}

void GagModel::componentComplete()
{
    refresh(RefreshAll);
}

int GagModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_gagList.count();
}

QVariant GagModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.row() < m_gagList.count());

    const GagObject &gag = m_gagList.at(index.row());

    switch (role) {
    case TitleRole:
        return gag.title();
    case UrlRole:
        return gag.url();
    case ImageUrlRole:
        // should use QUrl::isLocalFile() but it is introduced in Qt 4.8
        if (gag.imageUrl().scheme() != "file")
            return QUrl();
        return gag.imageUrl();
    case FullImageUrlRole:
        return gag.fullImageUrl();
    case GifImageUrlRole:
        if (gag.gifImageUrl().scheme() != "file")
            return QUrl();
        return gag.gifImageUrl();
    case ImageHeightRole:
        return gag.imageHeight();
    case VotesCountRole:
        return gag.votesCount();
    case CommentsCountRole:
        return gag.commentsCount();
    case IsVideoRole:
        return gag.isVideo();
    case IsNSFWRole:
        return gag.isNSFW();
    case IsGIFRole:
        return gag.isGIF();
    case IsPartialImageRole:
        return gag.isPartialImage();
    case IsDownloadingRole:
        return index.row() == m_downloadingIndex;
    default:
        qWarning("GagModel::data(): Invalid role");
        return QVariant();
    }
}

bool GagModel::isBusy() const
{
    return m_busy;
}

qreal GagModel::progress() const
{
    return m_progress;
}

int GagModel::gagCount() const
{
    return m_gagList.count();
}

GagBookManager *GagModel::manager() const
{
    return m_manager;
}

void GagModel::setManager(GagBookManager *manager)
{
    m_manager = manager;
}

int GagModel::selectedSection() const
{
    return m_selectedSection;
}

void GagModel::setSelectedSection(int selectedSection)
{
    if (m_selectedSection != selectedSection) {
        m_selectedSection = selectedSection;
        emit selectedSectionChanged();
    }
}

void GagModel::refresh(RefreshType refreshType)
{
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

    const QString section = m_manager->settings()->sections().at(m_selectedSection);

    switch (m_manager->settings()->source()) {
    default:
        qWarning("GagModel::refresh(): Invalid source, default source will be used");
        // fallthrough
    case AppSettings::NineGagSource:
    case AppSettings::InfiniGagSource:
        m_request = new NineGagRequest(manager()->networkManager(), section, this);
        break;
    /*case AppSettings::InfiniGagSource:
        qDebug() << "Using InfiniGagSource";
        m_request = new InfiniGagRequest(manager()->networkManager(), section, this);
        break;*/
    }

    if (!m_gagList.isEmpty()) {
        if (refreshType == RefreshAll) {
            beginRemoveRows(QModelIndex(), 0, m_gagList.count() - 1);
            m_gagList.clear();
            endRemoveRows();
        } else {
            m_request->setLastId(m_gagList.last().id());
        }
    }
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

void GagModel::stopRefresh()
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

void GagModel::downloadImage(int i)
{
    if (m_manualImageDownloader != 0) {
        m_manualImageDownloader->disconnect();
        m_manualImageDownloader->deleteLater();
        m_manualImageDownloader = 0;

        if (m_downloadingIndex != -1) {
            QModelIndex modelIndex = index(m_downloadingIndex);
            m_downloadingIndex = -1;
            emit dataChanged(modelIndex, modelIndex);
        }
    }

    m_downloadingIndex = i;
    emit dataChanged(index(i), index(i));

    QList<GagObject> gags;
    gags.append(m_gagList.at(i));

    m_manualImageDownloader = new GagImageDownloader(manager()->networkManager(), this);
    m_manualImageDownloader->setGagList(gags);
    m_manualImageDownloader->setDownloadGIF(gags.first().isGIF());
    connect(m_manualImageDownloader, SIGNAL(finished()), SLOT(onManualDownloadFinished()));
    m_manualImageDownloader->start();
}

void GagModel::onSuccess(const QList<GagObject> &gagList)
{
    m_imageDownloader = new GagImageDownloader(manager()->networkManager(), this);
    m_imageDownloader->setGagList(gagList);
    m_imageDownloader->setDownloadGIF(false);
    connect(m_imageDownloader, SIGNAL(downloadProgress(int,int)), SLOT(onImageDownloadProgress(int,int)));
    connect(m_imageDownloader, SIGNAL(finished()), SLOT(onDownloadFinished()));
    m_imageDownloader->start();

    m_request->deleteLater();
    m_request = 0;
}

void GagModel::onFailure(const QString &errorMessage)
{
    emit refreshFailure(errorMessage);
    m_request->deleteLater();
    m_request = 0;
    m_busy = false;
    emit busyChanged();
}

void GagModel::onImageDownloadProgress(int imagesDownloaded, int imagesTotal)
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

void GagModel::onDownloadFinished()
{
    const QList<GagObject> gagList = m_imageDownloader->gagList();
    beginInsertRows(QModelIndex(), m_gagList.count(), m_gagList.count() + gagList.count() - 1);
    m_gagList.reserve(m_gagList.count() + gagList.count());
    m_gagList.append(gagList);
    endInsertRows();

    m_busy = false;
    emit busyChanged();
    emit countChanged();

    m_imageDownloader->deleteLater();
    m_imageDownloader = 0;
}

void GagModel::onManualDownloadFinished()
{
    QModelIndex modelIndex = index(m_downloadingIndex);
    m_downloadingIndex = -1;
    emit dataChanged(modelIndex, modelIndex);

    m_manualImageDownloader->deleteLater();
    m_manualImageDownloader = 0;
}
