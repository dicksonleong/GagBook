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

#include "networkmanager.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkConfiguration>
#include <QtNetwork/QNetworkCookie>
#include <QDebug>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QUrlQuery>
#endif

#include "gagcookiejar.h"

static const QByteArray USER_AGENT = QByteArray("GagBook/") + APP_VERSION;

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent), m_networkAccessManager(new QNetworkAccessManager(this)),
    m_downloadCounter(0), m_downloadCounterStr("0.00")
{
    m_networkAccessManager->setCookieJar(new GagCookieJar);
    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));
}

QNetworkReply *NetworkManager::createGetRequest(const QUrl &url, AcceptType acceptType)
{
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("User-Agent", USER_AGENT);

    switch (acceptType) {
    case None: break;
    case JSON: request.setRawHeader("Accept", "application/json"); break;
    case HTML: request.setRawHeader("Accept", "text/html"); break;
    case Image: request.setRawHeader("Accept", "image/*"); break;
    default: qWarning("NetworkManager::createGetRequest(): Invalid acceptType"); break;
    }

    return m_networkAccessManager->get(request);
}

QNetworkReply *NetworkManager::createPostRequest(const QUrl &url, const QByteArray &data)
{
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("User-Agent", USER_AGENT);
    return m_networkAccessManager->post(request, data);
}

void NetworkManager::login(const QString username, const QString password) {
    QUrlQuery postData;
    postData.addQueryItem("username", username);
    postData.addQueryItem("password", password);

    QNetworkRequest request(QUrl("http://9gag.com/login"));

    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");

    m_networkAccessManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
}

bool NetworkManager::isMobileData() const
{
    const QNetworkConfiguration activeConfiguration = m_networkAccessManager->activeConfiguration();
    switch (activeConfiguration.bearerType()) {
    case QNetworkConfiguration::Bearer2G:
    case QNetworkConfiguration::BearerCDMA2000:
    case QNetworkConfiguration::BearerWCDMA:
    case QNetworkConfiguration::BearerHSPA:
        return true;
    default:
        return false;
    }
}

void NetworkManager::clearCookies()
{
    GagCookieJar *cookieJar = qobject_cast<GagCookieJar *>(m_networkAccessManager->cookieJar());
    Q_ASSERT(cookieJar != 0);
    cookieJar->clear();

    emit loggedInChanged();
}

QString NetworkManager::downloadCounter() const
{
    return m_downloadCounterStr;
}

void NetworkManager::replyFinished(QNetworkReply *reply)
{
    m_downloadCounter += reply->size();
    const QString downloadCounterStr = QString::number(qreal(m_downloadCounter) / 1024 / 1024, 'f', 2);
    if (m_downloadCounterStr != downloadCounterStr) {
        m_downloadCounterStr = downloadCounterStr;
        emit downloadCounterChanged();
    }

    //this is to handle login/logout only
    if(reply->error() == QNetworkReply::NoError) {
        // Get the http status code
        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (v >= 200 && v < 300) // Success
        {
            //no need to do anything
        }
        else if (v >= 300 && v < 400) // Redirection
        {
            // Redirection is expected when logging in, if login succesful we'll be directed to 9gag.com/

            // Get the redirection url
            QUrl newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            // Because the redirection url can be relative,
            // we have to use the previous one to resolve it
            newUrl = reply->url().resolved(newUrl);

            //save all the cookies we got from 9gag (including the loggedin one, which we'll use later)
            QVariant variantCookies = reply->header(QNetworkRequest::SetCookieHeader);
            QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(variantCookies);

            m_networkAccessManager->cookieJar()->setCookiesFromUrl(cookies, QUrl("9gag.com"));
        }

        //refresh the status in case we've been loggin in/logging out
        emit loggedInChanged();
    }
    else
    {
        // Error
        qDebug() << "Reply error: " << reply->errorString();
    }
}

GagCookieJar *NetworkManager::cookieJar() const
{
    return qobject_cast<GagCookieJar *>(m_networkAccessManager->cookieJar());
}
