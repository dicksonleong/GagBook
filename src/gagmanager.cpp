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

#include "gagmanager.h"

#include <QtNetwork/QNetworkAccessManager>

#include "gagmodel.h"
#include "gagrequest.h"
#include "settings.h"

GagManager::GagManager(QObject *parent) :
    QObject(parent), m_netManager(new QNetworkAccessManager(this)), m_request(0),
    m_busy(false), m_model(0), m_page(1)
{
    GagRequest::initializeCache();
}

void GagManager::refresh(RefreshType refreshType)
{
    Q_ASSERT_X(m_model != 0, Q_FUNC_INFO, "model not set");

    if (m_request != 0) {
        m_request->disconnect();
        m_request->deleteLater();
        m_request = 0;
    }

    if (m_model->isEmpty())
        refreshType = RefreshAll;

    GagRequest::Section selectedSection = static_cast<GagRequest::Section>(Settings::instance()->selectedSection());
    m_request = new GagRequest(selectedSection, m_netManager, this);

    if (refreshType == RefreshAll) {
        m_model->clear();
        m_page = 1;
    }
    else if (refreshType == RefreshOlder) {
        switch (selectedSection) {
        case GagRequest::Hot: case GagRequest::Trending: case GagRequest::Vote:
            m_request->setLastId(m_model->lastGagId());
            break;
        case GagRequest::TopDay: case GagRequest::TopWeek: case GagRequest::TopMonth: case GagRequest::TopAll:
            m_request->setPage(++m_page);
            break;
        }
    }

    connect(m_request, SIGNAL(success(QList<GagObject>)), this, SLOT(onSuccess(QList<GagObject>)));
    connect(m_request, SIGNAL(failure(QString)), this, SLOT(onFailure(QString)));

    m_request->send();

    setBusy(true);
}

bool GagManager::isBusy() const
{
    return m_busy;
}

void GagManager::setBusy(bool busy)
{
    if (m_busy != busy) {
        m_busy = busy;
        emit busyChanged();
    }
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

void GagManager::onSuccess(const QList<GagObject> &gagList)
{
    m_model->append(gagList);
    m_request->deleteLater();
    m_request = 0;
    setBusy(false);
}

void GagManager::onFailure(const QString &errorMessage)
{
    emit refreshFailure(errorMessage);
    m_request->deleteLater();
    m_request = 0;
    setBusy(false);
}
