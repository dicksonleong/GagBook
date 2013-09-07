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

#ifndef GAGREQUEST_H
#define GAGREQUEST_H

#include <QtCore/QObject>
#include <QtCore/QList>

#include "gagobject.h"
#include "gagsettings.h"

class NetworkManager;
class QNetworkReply;

class GagRequest : public QObject
{
    Q_OBJECT
public:
    explicit GagRequest(NetworkManager *networkManager, GagSettings::Section section, QObject *parent = 0);

    void setLastId(const QString &lastId);

    void send();

signals:
    void success(const QList<GagObject> &gagList);
    void failure(const QString &errorMessage);

protected:
    // must be override
    virtual QNetworkReply *createRequest(GagSettings::Section section, const QString &lastId) = 0;
    virtual QList<GagObject> parseResponse(const QByteArray &response) = 0;

    static QString getSectionText(GagSettings::Section section);
    NetworkManager *networkManager() const;

private slots:
    void onFinished();

private:
    NetworkManager *m_networkManager;
    const GagSettings::Section m_section;
    QString m_lastId;

    QNetworkReply *m_reply;
    QList<GagObject> m_gagList;
};

#endif // GAGREQUEST_H
