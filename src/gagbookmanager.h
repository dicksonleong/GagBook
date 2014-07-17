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

#ifndef GAGBOOKMANAGER_H
#define GAGBOOKMANAGER_H

#include <QtCore/QObject>

class NetworkManager;
class AppSettings;

class GagBookManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString downloadCounter READ downloadCounter NOTIFY downloadCounterChanged)
    Q_PROPERTY(AppSettings *settings READ settings WRITE setSettings)
    Q_PROPERTY(bool loggedIn READ isLoggedIn NOTIFY loggedInChanged)
public:
    explicit GagBookManager(QObject *parent = 0);

    QString downloadCounter() const;

    AppSettings *settings() const;
    void setSettings(AppSettings *settings);

    NetworkManager *networkManager() const;

    bool isLoggedIn() const;

    Q_INVOKABLE void login(const QString &username, const QString &password);
    Q_INVOKABLE void logout();

signals:
    void downloadCounterChanged();
    void loggedInChanged();

private:
    AppSettings *m_settings;
    NetworkManager *m_netManager;
};

#endif // GAGBOOKMANAGER_H
