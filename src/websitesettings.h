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

#ifndef WEBSITESETTINGS_H
#define WEBSITESETTINGS_H

#include <QtCore/QObject>

class QSettings;
class QNetworkReply;
class GagBookManager;

class WebsiteSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(bool cute READ cute WRITE setCute NOTIFY settingsChanged)
    Q_PROPERTY(bool geeky READ geeky WRITE setGeeky NOTIFY settingsChanged)
    Q_PROPERTY(bool gif READ gif WRITE setGif NOTIFY settingsChanged)
    Q_PROPERTY(bool showHotInTrending READ showHotInTrending WRITE setShowHotInTrending NOTIFY settingsChanged)
    Q_PROPERTY(bool showNsfw READ showNsfw WRITE setShowNsfw NOTIFY settingsChanged)
    Q_PROPERTY(GagBookManager *manager READ manager WRITE setManager)
public:
    explicit WebsiteSettings(QObject *parent = 0);
    ~WebsiteSettings();

    Q_INVOKABLE void save();
    Q_INVOKABLE void reset();

    bool isBusy() const;

    bool cute() const;
    void setCute(bool cute);

    bool geeky() const;
    void setGeeky(bool geeky);

    bool gif() const;
    void setGif(bool gif);

    bool showHotInTrending() const;
    void setShowHotInTrending(bool showHotInTrending);

    bool showNsfw() const;
    void setShowNsfw(bool showNsfw);

    GagBookManager *manager() const;
    void setManager(GagBookManager *manager);

signals:
    void success();
    void failure(const QString &message);
    void busyChanged();

    void settingsChanged();

private slots:
    void onFinished();

private:
    QSettings *m_settings;
    QNetworkReply *m_reply;

    bool m_busy;
    bool m_cute;
    bool m_geeky;
    bool m_gif;
    bool m_showHotInTrending;
    bool m_showNsfw;
    GagBookManager *m_manager;
};

#endif // WEBSITESETTINGS_H
