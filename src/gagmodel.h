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

/*! List model of GagObject list for QML. */
class GagModel : public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    DECL_QMLPARSERSTATUS_INTERFACE
    Q_ENUMS(RefreshType)

    /*! True if there is active refresh request. Busy visual feedback should show to user and
        refresh should be disable. */
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)

    /*! The current progress of downloading images. Value between 0.0 to 1.0. */
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)

    /*! The current progress of manually downloading image (using downloadImage()).
        Value between 0.0 to 1.0. */
    Q_PROPERTY(qreal manualProgress READ manualProgress NOTIFY manualProgressChanged)

    /*! Set the global instance of GagBookManager. Must be set before component completed
        and can not be change afterward. */
    Q_PROPERTY(GagBookManager *manager READ manager WRITE setManager)

    /*! The selected 9GAG section to get the gags. */
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
        LikesRole,
        IsNSFWRole,
        IsGIFRole,
        IsPartialImageRole,
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

    /*! Refresh the gag list. */
    Q_INVOKABLE void refresh(RefreshType refreshType);
    /*! Stop and abort the refresh request. */
    Q_INVOKABLE void stopRefresh();
    /*! Download an image for a gag at index \p i. If the gag is a GIF,
        the GIF image is download instead. */
    Q_INVOKABLE void downloadImage(int i);
    /*! Change the `likes` of a gag with the \p id. */
    Q_INVOKABLE void changeLikes(const QString &id, int likes);

signals:
    void busyChanged();
    void progressChanged();
    void manualProgressChanged();
    void selectedSectionChanged();

    /*! Emit when refresh failed, \p errorMessage contains the reason for the
        failure and should show to user. */
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
