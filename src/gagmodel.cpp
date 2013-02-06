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

#include "gagmodel.h"

#include <QtNetwork/QNetworkAccessManager>

GagModel::GagModel(QObject *parent) :
    QAbstractListModel(parent), m_netManager(new QNetworkAccessManager(this)), m_gagRequest(0)
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[ImageUrlRole] = "imageUrl";
    roles[VotesCountRole] = "votesCount";
    setRoleNames(roles);
}

int GagModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_gagList.count();
}

QVariant GagModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.row() < m_gagList.count());

    const GagObject gag = m_gagList.at(index.row());

    switch (role) {
    case TitleRole:
        return gag.title();
    case ImageUrlRole:
        return gag.imageUrl();
    case VotesCountRole:
        return gag.votesCount();
    default:
        qWarning("GagModel::data(): Invalid role");
        return QVariant();
    }
}

void GagModel::refresh(RefreshType refreshType)
{
    if (m_gagRequest != 0) {
        m_gagRequest->disconnect();
        m_gagRequest->deleteLater();
        m_gagRequest = 0;
    }

    if (m_gagList.isEmpty())
        refreshType = RefreshAll;
    else if (refreshType == RefreshAll)
        clearModel();

    m_gagRequest = new GagRequest(m_netManager, this);
    m_gagRequest->setSection(m_section);
    if (refreshType == RefreshOlder)
        m_gagRequest->setLastId(m_gagList.last().id());

    connect(m_gagRequest, SIGNAL(success(QList<GagObject>)), this, SLOT(onSuccess(QList<GagObject>)));
    connect(m_gagRequest, SIGNAL(failure(QString)), this, SLOT(onFailure(QString)));

    m_gagRequest->send();

    setBusy(true);
}

QVariantMap GagModel::get(int rowIndex)
{
    Q_ASSERT_X(rowIndex < m_gagList.count(), Q_FUNC_INFO, "rowIndex out of range");
    return m_gagList.at(rowIndex).toVariantMap();
}

bool GagModel::busy() const
{
    return m_busy;
}

void GagModel::setBusy(bool busy)
{
    if (m_busy != busy) {
        m_busy = busy;
        emit busyChanged();
    }
}

GagRequest::Section GagModel::section() const
{
    return m_section;
}

void GagModel::setSection(GagRequest::Section section)
{
    if (m_section != section) {
        m_section = section;
        emit sectionChanged();
    }
}

void GagModel::onSuccess(const QList<GagObject> &gagList)
{
    beginInsertRows(QModelIndex(), m_gagList.count(), m_gagList.count() + gagList.count() - 1);
    m_gagList.append(gagList);
    endInsertRows();

    m_gagRequest->deleteLater();
    m_gagRequest = 0;
    setBusy(false);
}

void GagModel::onFailure(const QString &errorMessage)
{
    emit failure(errorMessage);
    m_gagRequest->deleteLater();
    m_gagRequest = 0;
    setBusy(false);
}

inline void GagModel::clearModel()
{
    Q_ASSERT(!m_gagList.isEmpty());

    beginRemoveRows(QModelIndex(), 0, m_gagList.count() - 1);
    m_gagList.clear();
    endRemoveRows();
}
