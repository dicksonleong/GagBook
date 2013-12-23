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

#include "websitesettings.h"

#include <QtCore/QSettings>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "gagbookmanager.h"
#include "networkmanager.h"

WebsiteSettings::WebsiteSettings(QObject *parent) :
    QObject(parent), m_settings(new QSettings(this)), m_reply(0), m_busy(false), m_manager(0)
{
    m_settings->beginGroup("Website");
    m_cute = m_settings->value("cute", true).toBool();
    m_geeky = m_settings->value("geeky", true).toBool();
    m_gif = m_settings->value("gif", true).toBool();
    m_showHotInTrending = m_settings->value("showHotInTrending", true).toBool();
    m_showNsfw = m_settings->value("showNsfw", true).toBool();
}

WebsiteSettings::~WebsiteSettings()
{
    if (m_reply != 0) {
        m_reply->disconnect();
        m_reply->deleteLater();
        m_reply = 0;
    }
}

void WebsiteSettings::save()
{
    Q_ASSERT_X(m_manager != 0, Q_FUNC_INFO, "manager not set");

    if (m_reply != 0) {
        m_reply->disconnect();
        m_reply->deleteLater();
        m_reply = 0;
    }

    m_settings->setValue("cute", m_cute);
    m_settings->setValue("geeky", m_geeky);
    m_settings->setValue("gif", m_gif);
    m_settings->setValue("showHotInTrending", m_showHotInTrending);
    m_settings->setValue("showNsfw", m_showNsfw);

    QUrl requestUrl("http://9gag.com/profile/setcustomize");

    QList< QPair<QString,QString> > dataList;
    dataList << qMakePair(QString("section_23"), QString(m_cute ? "1" : ""));
    dataList << qMakePair(QString("section_25"), QString(m_geeky ? "1" : ""));
    dataList << qMakePair(QString("section_30"), QString(m_gif ? "1" : ""));
    dataList << qMakePair(QString("show_hot"), QString(m_showHotInTrending ? "1" : ""));
    dataList << qMakePair(QString("show_nsfw"), QString(m_showNsfw ? "1" : ""));

    QUrl dummyUrl("http://dummy.com");
    dummyUrl.setQueryItems(dataList);

    m_reply = m_manager->networkManager()->createPostRequest(requestUrl, dummyUrl.encodedQuery());
    connect(m_reply, SIGNAL(finished()), SLOT(onFinished()));

    m_busy = true;
    emit busyChanged();
}

void WebsiteSettings::reset()
{
    Q_ASSERT_X(m_manager != 0, Q_FUNC_INFO, "manager not set");

    if (m_reply != 0) {
        m_reply->disconnect();
        m_reply->deleteLater();
        m_reply = 0;
    }

    m_cute = true;
    m_geeky = true;
    m_gif = true;
    m_showHotInTrending = true;
    m_showNsfw = true;
    m_settings->setValue("cute", m_cute);
    m_settings->setValue("geeky", m_geeky);
    m_settings->setValue("gif", m_gif);
    m_settings->setValue("showHotInTrending", m_showHotInTrending);
    m_settings->setValue("showNsfw", m_showNsfw);
    emit settingsChanged();

    QUrl requestUrl("http://9gag.com/profile/setcustomize");
    m_reply = m_manager->networkManager()->createPostRequest(requestUrl, "customize_reset=1");
    connect(m_reply, SIGNAL(finished()), SLOT(onFinished()));

    m_busy = true;
    emit busyChanged();
}

bool WebsiteSettings::isBusy() const
{
    return m_busy;
}

bool WebsiteSettings::cute() const
{
    return m_cute;
}

void WebsiteSettings::setCute(bool cute)
{
    m_cute = cute;
}

bool WebsiteSettings::geeky() const
{
    return m_geeky;
}

void WebsiteSettings::setGeeky(bool geeky)
{
    m_geeky = geeky;
}

bool WebsiteSettings::gif() const
{
    return m_gif;
}

void WebsiteSettings::setGif(bool gif)
{
    m_gif = gif;
}

bool WebsiteSettings::showHotInTrending() const
{
    return m_showHotInTrending;
}

void WebsiteSettings::setShowHotInTrending(bool showHotInTrending)
{
    m_showHotInTrending = showHotInTrending;
}

bool WebsiteSettings::showNsfw() const
{
    return m_showNsfw;
}

void WebsiteSettings::setShowNsfw(bool showNsfw)
{
    m_showNsfw = showNsfw;
}

GagBookManager *WebsiteSettings::manager() const
{
    return m_manager;
}

void WebsiteSettings::setManager(GagBookManager *manager)
{
    m_manager = manager;
}

void WebsiteSettings::onFinished()
{
    if (m_reply->error())
        emit failure("Error: " + m_reply->errorString());
    else
        emit success();

    m_reply->deleteLater();
    m_reply = 0;

    m_busy = false;
    emit busyChanged();
}
