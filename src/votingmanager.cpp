/*
 * Copyright (c) 2014 Bob Jelica
 * Copyright (c) 2014 Dickson Leong
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

#include "votingmanager.h"

#include <QMetaEnum>
#include <QtNetwork/QNetworkReply>
#include <../qt-json/json.h>
#include <QDebug>

#include "gagbookmanager.h"
#include "networkmanager.h"

VotingManager::VotingManager(QObject *parent) :
    QObject(parent), m_isBusy(false), m_manager(0), m_reply(0)
{  
}

bool VotingManager::isBusy() const
{
    return m_isBusy;
}

GagBookManager *VotingManager::manager() const
{
    return m_manager;
}

void VotingManager::setManager(GagBookManager *manager)
{
    m_manager = manager;
}

void VotingManager::vote(const QString &id, VotingManager::VoteType voteType)
{
    Q_ASSERT(m_manager != 0);

    if (m_reply != 0) {
        m_reply->disconnect();
        m_reply->deleteLater();
        m_reply = 0;
    }

    if (m_isBusy != true) {
        m_isBusy = true;
        emit busyChanged();
    }

    QUrl voteUrl("http://9gag.com/vote/"+ enumToString(voteType).toLower() +"/id/" + id);
    m_reply = m_manager->networkManager()->createPostRequest(voteUrl, QByteArray());
    m_reply->setParent(this);
    connect(m_reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
}

void VotingManager::onReplyFinished()
{
    if (m_reply->error() == QNetworkReply::NoError) {
        bool ok;
        const QVariantMap result = QtJson::parse(QString::fromUtf8(m_reply->readAll()), ok).toMap();

        Q_ASSERT_X(ok, Q_FUNC_INFO, "Error parsing JSON");
        Q_ASSERT_X(!result.isEmpty(), Q_FUNC_INFO, "Error parsing JSON or JSON is empty");

        const QString id = result.value("id").toString();
        const int score = result.value("myScore").toInt();
        const QString msg = result.value("msg").toString();

        if (msg.contains("loved", Qt::CaseInsensitive)) // if success, msg can be "Loved" or "Not loved"
            emit voteSuccess(id, score);
         else
            emit failure(msg);
    } else {
        emit failure(m_reply->errorString());
    }

    m_reply->deleteLater();
    m_reply = 0;

    if (m_isBusy != false) {
        m_isBusy = false;
        emit busyChanged();
    }
}


QString VotingManager::enumToString(VoteType aElement)
{
    int index = metaObject()->indexOfEnumerator("VoteType");
    QMetaEnum metaEnum = metaObject()->enumerator(index);
    return metaEnum.valueToKey(aElement);
}
