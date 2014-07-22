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

#include "gagbookmanager.h"

#include <QtCore/QDateTime>
#include <QtNetwork/QNetworkCookieJar>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkReply>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtCore/QUrlQuery>
#endif

#include "networkmanager.h"
#include "gagimagedownloader.h"
#include "appsettings.h"

GagBookManager::GagBookManager(QObject *parent) :
    QObject(parent), m_isLoggedIn(false), m_settings(0), m_netManager(new NetworkManager(this)), m_loginReply(0)
{
    GagImageDownloader::initializeCache();
    connect(m_netManager, SIGNAL(downloadCounterChanged()), SIGNAL(downloadCounterChanged()));
    m_isLoggedIn = checkIsLoggedIn();
}

bool GagBookManager::isLoggedIn() const
{
    return m_isLoggedIn;
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
    Q_ASSERT(m_netManager);
    m_settings->setUsername(username); //save username so users don't have to input it everytime

    if (m_loginReply != 0) {
        m_loginReply->disconnect();
        m_loginReply->deleteLater();
        m_loginReply = 0;
    }

    QUrlQuery postData;
    postData.addQueryItem("username", username);
    postData.addQueryItem("password", password);

    m_loginReply = m_netManager->createPostRequest(QUrl("https://9gag.com/login"),
                                                   postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(m_loginReply, SIGNAL(finished()), SLOT(onLoginFinished()));
}

void GagBookManager::logout()
{
    //we log out by removing the loggedin cookie
    m_netManager->clearCookies();
    if (m_isLoggedIn != false) {
        m_isLoggedIn = false;
        emit loggedInChanged();
    }
}

void GagBookManager::onLoginFinished()
{
    if(m_loginReply->error() == QNetworkReply::NoError) {
        if (checkIsLoggedIn()) {
            m_isLoggedIn = true;
            emit loggedInChanged();
        } else {
            qWarning("GagBookManager::onLoginFinished(): Login failed, probably because wrong username/password");
        }
    } else {
        qWarning("GagBookManager::onLoginFinished(): Login error: %s", qPrintable(m_loginReply->errorString()));
    }

    m_loginReply->deleteLater();
    m_loginReply = 0;
}

bool GagBookManager::checkIsLoggedIn()
{
    const QList<QNetworkCookie> cookies = m_netManager->cookieJar()->cookiesForUrl((QUrl("http://9gag.com/")));

    foreach (const QNetworkCookie &cookie, cookies) {
        if (cookie.name() == "loggedin" && cookie.value() == "1"
                && cookie.expirationDate() > QDateTime::currentDateTime())
            return true;
    }

    return false;
}
