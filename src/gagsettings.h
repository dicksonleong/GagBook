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

#ifndef GAGSETTINGS_H
#define GAGSETTINGS_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

class QSettings;

class GagSettings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int selectedSection READ selectedSection WRITE setSelectedSection NOTIFY selectedSectionChanged)
    Q_PROPERTY(bool whiteTheme READ isWhiteTheme WRITE setWhiteTheme NOTIFY whiteThemeChanged)
    Q_PROPERTY(int source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(int autoDownloadGif READ autoDownloadGif WRITE setAutoDownloadGif NOTIFY autoDownloadGifChanged)
public:
    static GagSettings *instance();

    int selectedSection() const;
    void setSelectedSection(int selectedSection);

    bool isWhiteTheme() const;
    void setWhiteTheme(bool whiteTheme);

    int source() const;
    void setSource(int source);

    int autoDownloadGif() const;
    void setAutoDownloadGif(int autoDownloadGif);

signals:
    void selectedSectionChanged();
    void whiteThemeChanged();
    void sourceChanged();
    void autoDownloadGifChanged();

private:
    static QScopedPointer<GagSettings> m_instance;

    explicit GagSettings(QObject *parent = 0);
    Q_DISABLE_COPY(GagSettings)

    QSettings *m_settings;

    int m_selectedSection;
    bool m_whiteTheme;
    int m_source;
    int m_autoDownloadGif;
};

#endif // GAGSETTINGS_H
