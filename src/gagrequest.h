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

#ifndef GAGREQUEST_H
#define GAGREQUEST_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtWebKit/QWebPage>

#include "gagobject.h"

class QNetworkAccessManager;
class QNetworkReply;
class QWebElementCollection;

class GagRequest : public QObject
{
    Q_OBJECT
    Q_ENUMS(Section)
public:
    explicit GagRequest(QNetworkAccessManager *manager, QObject *parent = 0);
    ~GagRequest();

    enum Section { Hot, Trending, Vote };

    void setSection(Section section);
    void setLastId(int lastId);

    void send();

signals:
    void success(const QList<GagObject> &gagList);
    void failure(const QString &errorMessage);

private slots:
    void onFinished();

private:
    QNetworkAccessManager *m_netManager;
    QNetworkReply *m_reply;

    Section m_section;
    int m_lastId;

    QWebPage m_webPage;
    QList<GagObject> parsedGagList;

    void parseGAG(const QWebElementCollection &entryItems);
    void parseVoteGAG(const QWebElementCollection &entryItems);

    static QString getSectionText(Section section);
};

#endif // GAGREQUEST_H
