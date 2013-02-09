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
 *     * Neither the name of GagBook nor the names of its contributors may be
 *       used to endorse or promote products derived from this software without
 *       specific prior written permission.
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

#include "gagrequest.h"
#include "gagobject.h"

class QNetworkAccessManager;

class GagModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(GagRequest::Section)
    Q_ENUMS(RefreshType)

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(GagRequest::Section section READ section WRITE setSection NOTIFY sectionChanged)
public:
    explicit GagModel(QObject *parent = 0);

    enum Roles {
        TitleRole = Qt::UserRole,
        ImageUrlRole,
        VotesCountRole,
        IsVideoRole,
        IsNSFWRole
    };

    enum RefreshType {
        RefreshAll,
        RefreshOlder
    };

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE void refresh(RefreshType refreshType);
    Q_INVOKABLE QVariantMap get(int rowIndex);

    bool busy() const;
    void setBusy(bool busy);

    GagRequest::Section section() const;
    void setSection(GagRequest::Section section);

signals:
    void failure(const QString &errorMessage);

    void busyChanged();
    void sectionChanged();
    
private slots:
    void onSuccess(const QList<GagObject> &gagList);
    void onFailure(const QString &errorMessage);

private:
    QNetworkAccessManager *m_netManager;
    GagRequest *m_gagRequest;
    QList<GagObject> m_gagList;

    bool m_busy;
    GagRequest::Section m_section;

    inline void clearModel();
};

#endif // GAGMODEL_H
