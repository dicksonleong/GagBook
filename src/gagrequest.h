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

#ifndef GAGREQUEST_H
#define GAGREQUEST_H

#include <QtCore/QObject>
#include <QtCore/QList>

#include "gagobject.h"
#include "gagmodel.h"

class NetworkManager;
class QNetworkReply;

/*! Download list of gag

    A abstract class that encapsulate a request to download a list of gag. Can be
    subclass to support different ways of getting gags.

    \sa NineGagRequest, InfiniGagRequest
 */
class GagRequest : public QObject
{
    Q_OBJECT
public:
    /*! Constructor. \p section specify from which 9GAG section to get the gags, eg. hot, comic, etc. */
    explicit GagRequest(NetworkManager *networkManager, const QString &section, QObject *parent = 0);

    /*! Set the id of the last gag in the list. If this is set, the gags retrieved are older than
        the last gag. */
    void setLastId(const QString &lastId);

    /*! Send the request. */
    void send();

signals:
    /*! Emit when the request is succeeded. \p gagList contains the gag list retrieved. */
    void success(const QList<GagObject> &gagList);

    /*! Emit when the request is failed, \p errorMessage contains the reason
        of the failure and should be shown to user. */
    void failure(const QString &errorMessage);

protected:
    /*! Implement this to make your own network request. \p section specify from which
        9GAG section to get the gags, eg. hot, comic, etc. \p lastId is id of the last
        gag, can be empty. */
    virtual QNetworkReply *createRequest(const QString &section, const QString &lastId) = 0;

    /*! Implement this to parse the response to a list of gags. */
    virtual QList<GagObject> parseResponse(const QByteArray &response) = 0;

    /*! Get the global instance of NetworkManager. */
    NetworkManager *networkManager() const;

private slots:
    void onFinished();

private:
    NetworkManager *m_networkManager;
    const QString m_section;
    QString m_lastId;

    QNetworkReply *m_reply;
    QList<GagObject> m_gagList;
};

#endif // GAGREQUEST_H
