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
#include <QtCore/QSize>

#include "gagbookmanager.h"
#include "appsettings.h"
#include "networkmanager.h"
#include "infinigagrequest.h"
#include "ninegagrequest.h"
#include "gagimagedownloader.h"

GagModel::GagModel(QObject *parent) :
    QAbstractListModel(parent), m_busy(false), m_progress(0), m_manualProgress(0), m_manager(0),
    m_selectedSection(0), m_request(0), m_imageDownloader(0), m_manualImageDownloader(0), m_downloadingIndex(-1)
{
    _roles[TitleRole] = "title";
    _roles[IdRole] = "id";
    _roles[UrlRole] = "url";
    _roles[ImageUrlRole] = "imageUrl";
    _roles[FullImageUrlRole] = "fullImageUrl";
    _roles[GifImageUrlRole] = "gifImageUrl";
    _roles[ImageSizeRole] = "imageSize";
    _roles[VotesCountRole] = "votesCount";
    _roles[CommentsCountRole] = "commentsCount";
    _roles[LikesRole] = "likes";
    _roles[IsNSFWRole] = "isNSFW";
    _roles[IsGIFRole] = "isGIF";
    _roles[IsPartialImageRole] = "isPartialImage";
    _roles[IsDownloadingRole] = "isDownloading";
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    setRoleNames(_roles);
#endif
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
    case IdRole:
        return gag.id();
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
    case ImageSizeRole:
        return gag.imageSize();
    case VotesCountRole:
        return gag.votesCount();
    case CommentsCountRole:
        return gag.commentsCount();
    case LikesRole:
        return gag.likes();
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

QHash<int, QByteArray> GagModel::roleNames() const
{
    return _roles;
}

bool GagModel::isBusy() const
{
    return m_busy;
}

qreal GagModel::progress() const
{
    return m_progress;
}

qreal GagModel::manualProgress() const
{
    return m_manualProgress;
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
        m_request = new NineGagRequest(manager()->networkManager(), section, this);
        break;
    case AppSettings::InfiniGagSource:
        m_request = new InfiniGagRequest(manager()->networkManager(), section, this);
        break;
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

    if (m_busy != true) {
        m_busy = true;
        emit busyChanged();
    }

    if (m_progress != 0) {
        m_progress = 0;
        emit progressChanged();
    }

    m_request->send();
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

    if (m_manualProgress != 0) {
        m_manualProgress = 0;
        emit manualProgressChanged();
    }

    QList<GagObject> gags;
    gags.append(m_gagList.at(i));

    m_manualImageDownloader = new GagImageDownloader(manager()->networkManager(), this);
    m_manualImageDownloader->setGagList(gags);
    m_manualImageDownloader->setDownloadGIF(gags.first().isGIF());
    connect(m_manualImageDownloader, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(onManualDownloadProgress(qint64,qint64)));
    connect(m_manualImageDownloader, SIGNAL(finished()), SLOT(onManualDownloadFinished()));
    m_manualImageDownloader->start();
}

void GagModel::changeLikes(const QString &id, int likes)
{
    for (int i = 0; i < m_gagList.count(); ++i) {
        GagObject gag = m_gagList.at(i);
        if (gag.id() == id) {
            int oldLikes = gag.likes();
            gag.setLikes(likes);
            gag.setVotesCount(gag.votesCount() + (likes - oldLikes));
            emit dataChanged(index(i), index(i));
            break;
        }
    }
}

void GagModel::onSuccess(const QList<GagObject> &gagList)
{
    m_imageDownloader = new GagImageDownloader(manager()->networkManager(), this);
    m_imageDownloader->setGagList(gagList);
    m_imageDownloader->setDownloadGIF(false);
    connect(m_imageDownloader, SIGNAL(downloadProgress(qint64,qint64)), SLOT(onDownloadProgress(qint64,qint64)));
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

    if (m_busy != false) {
        m_busy = false;
        emit busyChanged();
    }
}

void GagModel::onDownloadProgress(qint64 downloaded, qint64 total)
{
    qreal progress;
    if (total > 0)
        progress = qreal(downloaded) / qreal(total);
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

    if (m_busy != false) {
        m_busy = false;
        emit busyChanged();
    }

    m_imageDownloader->deleteLater();
    m_imageDownloader = 0;
}

void GagModel::onManualDownloadProgress(qint64 downloaded, qint64 total)
{
    qreal progress;
    if (total > 0)
        progress = qreal(downloaded) / qreal(total);
    else
        progress = 1;

    if (m_manualProgress != progress) {
        m_manualProgress = progress;
        emit manualProgressChanged();
    }
}

void GagModel::onManualDownloadFinished()
{
    QModelIndex modelIndex = index(m_downloadingIndex);
    m_downloadingIndex = -1;
    emit dataChanged(modelIndex, modelIndex);

    m_manualImageDownloader->deleteLater();
    m_manualImageDownloader = 0;
}
