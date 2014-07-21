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

#ifndef GAGMODEL_H
#define GAGMODEL_H

#include <QtCore/QAbstractListModel>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  #include <QtQml/QQmlParserStatus>
  #define DECL_QMLPARSERSTATUS_INTERFACE Q_INTERFACES(QQmlParserStatus)
#else
  #include <QtDeclarative/QDeclarativeParserStatus>
  #define QQmlParserStatus QDeclarativeParserStatus
  #define DECL_QMLPARSERSTATUS_INTERFACE Q_INTERFACES(QDeclarativeParserStatus)
#endif

#include "gagobject.h"

class GagBookManager;
class GagRequest;
class GagImageDownloader;

class GagModel : public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    DECL_QMLPARSERSTATUS_INTERFACE
    Q_ENUMS(RefreshType)
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(qreal manualProgress READ manualProgress NOTIFY manualProgressChanged)
    Q_PROPERTY(GagBookManager *manager READ manager WRITE setManager)
    Q_PROPERTY(int selectedSection READ selectedSection WRITE setSelectedSection NOTIFY selectedSectionChanged)
public:
    enum Roles {
        TitleRole = Qt::UserRole,
        IdRole,
        UrlRole,
        ImageUrlRole,
        FullImageUrlRole,
        GifImageUrlRole,
        ImageSizeRole,
        VotesCountRole,
        CommentsCountRole,
        IsVideoRole,
        IsNSFWRole,
        IsGIFRole,
        IsPartialImageRole,
        IsLikedRole,
        IsDislikedRole,
        IsDownloadingRole
    };

    enum RefreshType {
        RefreshAll,
        RefreshOlder
    };

    explicit GagModel(QObject *parent = 0);

    void classBegin();
    void componentComplete();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    bool isBusy() const;
    qreal progress() const;
    qreal manualProgress() const;

    GagBookManager *manager() const;
    void setManager(GagBookManager *manager);

    int selectedSection() const;
    void setSelectedSection(int selectedSection);

    Q_INVOKABLE void refresh(RefreshType refreshType);
    Q_INVOKABLE void stopRefresh();
    Q_INVOKABLE void downloadImage(int i);

signals:
    void busyChanged();
    void progressChanged();
    void manualProgressChanged();
    void selectedSectionChanged();
    void refreshFailure(const QString &errorMessage);

private slots:
    void onSuccess(const QList<GagObject> &gagList);
    void onFailure(const QString &errorMessage);
    void onDownloadProgress(qint64 downloaded, qint64 total);
    void onDownloadFinished();
    void onManualDownloadProgress(qint64 downloaded, qint64 total);
    void onManualDownloadFinished();

private:
    bool m_busy;
    qreal m_progress;
    qreal m_manualProgress;
    GagBookManager *m_manager;
    int m_selectedSection;

    QHash<int, QByteArray> _roles;
    QList<GagObject> m_gagList;
    GagRequest *m_request;
    GagImageDownloader *m_imageDownloader;
    GagImageDownloader *m_manualImageDownloader;
    int m_downloadingIndex;
};

#endif // GAGMODEL_H
