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
class QNetworkReply;

/*! Handle login and hold other global instance class

    A single global instance class that hold and expose other global instance
    classes (AppSettings and NetworkManager) so they can be easily accessible
    by other classes that need them. Also handle 9GAG account login.
 */
class GagBookManager : public QObject
{
    Q_OBJECT

    /*! True if user is logged in to 9GAG account, otherwise false. */
    Q_PROPERTY(bool loggedIn READ isLoggedIn NOTIFY loggedInChanged)

    /*! True if there is an active login request. Busy visual feedback should
        show to user and login should be disable when busy. */
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)

    /*! The download counter for current app session, in MB with 2 decimal points. */
    Q_PROPERTY(QString downloadCounter READ downloadCounter NOTIFY downloadCounterChanged)

    /*! The global instance of AppSettings. Must be set before component completed and
        can not be change afterward. */
    Q_PROPERTY(AppSettings *settings READ settings WRITE setSettings)
public:
    explicit GagBookManager(QObject *parent = 0);

    bool isLoggedIn() const;
    bool isBusy() const;
    QString downloadCounter() const;

    /*! Get the global instance of AppSettings. */
    AppSettings *settings() const;
    void setSettings(AppSettings *settings);

    /*! Get the global instance of NetworkManager. */
    NetworkManager *networkManager() const;

    /*! Login to 9GAG account. If login success, loginSuccess() will emit, otherwise
        loginFailure() will emit. */
    Q_INVOKABLE void login(const QString &username, const QString &password);

    /*! Logout from 9GAG account. */
    Q_INVOKABLE void logout();

signals:
    void loggedInChanged();
    void busyChanged();
    void downloadCounterChanged();

    /*! Emit when login is succeeded. */
    void loginSuccess();

    /*! Emit when login is failed, \p errorMessage contains the reason for the
        failure and should show to user. */
    void loginFailure(const QString &errorMessage);

private slots:
    void onLoginFinished();

private:
    bool m_isLoggedIn;
    bool m_isBusy;
    AppSettings *m_settings;
    NetworkManager *m_netManager;
    QNetworkReply *m_loginReply;

    bool checkIsLoggedIn();
};

#endif // GAGBOOKMANAGER_H
