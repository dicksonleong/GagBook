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

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

class QSettings;

class AppSettings : public QObject
{
    Q_OBJECT
    Q_ENUMS(Section)
    Q_ENUMS(Source)
    Q_ENUMS(GifDownloadMode)

    Q_PROPERTY(bool whiteTheme READ isWhiteTheme WRITE setWhiteTheme NOTIFY whiteThemeChanged)
    Q_PROPERTY(Source source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(GifDownloadMode gifDownloadMode READ gifDownloadMode WRITE setGifDownloadMode
               NOTIFY gifDownloadModeChanged)
public:
    enum Source {
        NineGagSource,
        InfiniGagSource
    };

    enum GifDownloadMode {
        GifDownloadOn,
        GifDownloadOnWiFiOnly,
        GifDownloadOff
    };

    explicit AppSettings(QObject *parent = 0);

    bool isWhiteTheme() const;
    void setWhiteTheme(bool whiteTheme);

    Source source() const;
    void setSource(Source source);

    GifDownloadMode gifDownloadMode() const;
    void setGifDownloadMode(GifDownloadMode mode);

signals:
    void whiteThemeChanged();
    void sourceChanged();
    void gifDownloadModeChanged();

private:
    Q_DISABLE_COPY(AppSettings)

    QSettings *m_settings;
    bool m_whiteTheme;
    Source m_source;
    GifDownloadMode m_gifDownloadMode;
};

#endif // APPSETTINGS_H
