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

#include <QNetworkCookie>
#include <QDateTime>

#include "gagbookmanager.h"

#include "networkmanager.h"
#include "gagimagedownloader.h"
#include "appsettings.h"
#include "gagcookiejar.h"

#include <QDebug>

GagBookManager::GagBookManager(QObject *parent) :
    QObject(parent), m_settings(0), m_netManager(new NetworkManager(this))
{
    GagImageDownloader::initializeCache();
    connect(m_netManager, SIGNAL(downloadCounterChanged()), SIGNAL(downloadCounterChanged()));
    connect(m_netManager, SIGNAL(loggedInChanged()), SIGNAL(loggedInChanged()));
}

QString GagBookManager::downloadCounter() const
{
    return m_netManager->downloadCounter();
}

AppSettings *GagBookManager::settings() const
{
    return m_settings;
}

void GagBookManager::setSettings(AppSettings *settings)
{
    m_settings = settings;
}

NetworkManager *GagBookManager::networkManager() const
{
    return m_netManager;
}

void GagBookManager::login(const QString &username, const QString &password)
{
    qDebug() << Q_FUNC_INFO;
    Q_ASSERT(m_netManager);
    m_settings->setUsername(username); //save username so users don't have to input it everytime
    m_netManager->login(username, password);
}

void GagBookManager::logout()
{
    qDebug() << Q_FUNC_INFO;
    //we log out by removing the loggedin cookie
    m_netManager->clearCookies();

    emit loggedInChanged();
}

bool GagBookManager::isLoggedIn() const
{
    bool m_loggedIn = false;
    QList<QNetworkCookie> m_cookies = m_netManager->cookieJar()->cookiesForUrl((QUrl("9gag.com")));

    foreach (QNetworkCookie cookie, m_cookies) {
        QDateTime now = QDateTime::currentDateTime();
        if (cookie.expirationDate() < now)
            continue;

        m_loggedIn = (cookie.name() == "loggedin" && cookie.value() == "1");
    }

    return m_loggedIn;
}
