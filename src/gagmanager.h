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

#ifndef GAGMANAGER_H
#define GAGMANAGER_H

#include <QtCore/QObject>

#include "gagobject.h"

class GagRequest;
class GagModel;
class GagImageDownloader;

class GagManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(RefreshType)

    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(GagModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int downloadingImageIndex READ downloadingImageIndex NOTIFY downloadingImageIndexChanged)
public:
    explicit GagManager(QObject *parent = 0);

    enum RefreshType {
        RefreshAll,
        RefreshOlder
    };

    Q_INVOKABLE void refresh(RefreshType refreshType);
    Q_INVOKABLE void stopRefresh();
    Q_INVOKABLE void downloadImage(int index);

    bool isBusy() const;

    qreal progress() const;

    GagModel *model() const;
    void setModel(GagModel *model);

    int downloadingImageIndex() const;

signals:
    void refreshFailure(const QString &errorMessage);
    void busyChanged();
    void progressChanged();
    void modelChanged();
    void downloadingImageIndexChanged();

private slots:
    void onSuccess(const QList<GagObject> &gagList);
    void onFailure(const QString &errorMessage);
    void onImageDownloadProgress(int imagesDownloaded, int imagesTotal);
    void onDownloadFinished(const QList<GagObject> &gagList);
    void onManualDownloadFinished(const QList<GagObject> &gagList);

private:
    GagRequest *m_request;
    GagImageDownloader *m_imageDownloader;
    GagImageDownloader *m_manualImageDownloader;

    bool m_busy;
    qreal m_progress;
    GagModel *m_model;
    int m_downloadingImageIndex;
};

#endif // GAGMANAGER_H
