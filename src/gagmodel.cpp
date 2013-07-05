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

#include "gagmodel.h"

#include <QtCore/QUrl>

GagModel::GagModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[UrlRole] = "url";
    roles[ImageUrlRole] = "imageUrl";
    roles[ImageHeightRole] = "imageHeight";
    roles[VotesCountRole] = "votesCount";
    roles[CommentsCountRole] = "commentsCount";
    roles[IsVideoRole] = "isVideo";
    roles[IsNSFWRole] = "isNSFW";
    roles[IsGIFRole] = "isGIF";
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

    const GagObject &gag = m_gagList.at(index.row());

    switch (role) {
    case TitleRole:
        return gag.title();
    case UrlRole:
        return gag.url();
    case ImageUrlRole:
        return gag.imageUrl();
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
    default:
        qWarning("GagModel::data(): Invalid role");
        return QVariant();
    }
}

bool GagModel::isEmpty() const
{
    return m_gagList.isEmpty();
}

QString GagModel::lastGagId() const
{
    Q_ASSERT(!m_gagList.isEmpty());
    return m_gagList.last().id();
}

void GagModel::append(const QList<GagObject> &gagList)
{
    beginInsertRows(QModelIndex(), m_gagList.count(), m_gagList.count() + gagList.count() - 1);
    m_gagList.reserve(m_gagList.count() + gagList.count());
    m_gagList.append(gagList);
    endInsertRows();
}

void GagModel::clear()
{
    if (m_gagList.isEmpty())
        return;

    beginRemoveRows(QModelIndex(), 0, m_gagList.count() - 1);
    m_gagList.clear();
    endRemoveRows();
}

QVariantMap GagModel::get(int rowIndex) const
{
    Q_ASSERT_X(rowIndex < m_gagList.count(), Q_FUNC_INFO, "rowIndex out of range");
    return m_gagList.at(rowIndex).toVariantMap();
}
