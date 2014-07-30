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

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QStringList>

class QSettings;

/*! Expose QSettings keys as property

    Expose QSettings keys as property to allow easier access by both C++ and QML.
    Only a single global instance should be created for each app session. To add a
    new key to QSettings, add a Q_PROPERTY, a getter and setter method, a private
    member to store the value, and set the member from QSettings in contructor.
 */
class AppSettings : public QObject
{
    Q_OBJECT
    Q_ENUMS(Source)

    /*! True if white UI theme is used, false if dark UI theme (the default).
        Only usable for Harmattan and Symbian. */
    Q_PROPERTY(bool whiteTheme READ isWhiteTheme WRITE setWhiteTheme NOTIFY whiteThemeChanged)

    /*! Specify which source to use to get the gags. Default is NineGagSource. */
    Q_PROPERTY(Source source READ source WRITE setSource NOTIFY sourceChanged)

    /*! True if scroll with volume keys is enabled. Default is false. */
    Q_PROPERTY(bool scrollWithVolumeKeys READ scrollWithVolumeKeys WRITE setScrollWithVolumeKeys
               NOTIFY scrollWithVolumeKeysChanged)

    /*! List of 9GAG sections. This allow user to add/remove 9GAG sections manually and does not
        require an app update to view a newly added 9GAG sections. Currently there is no UI to
        modify this, that means user has to edit the config file manually. */
    Q_PROPERTY(QStringList sections READ sections WRITE setSections NOTIFY sectionsChanged)
public:
    enum Source {
        NineGagSource, //!< Use 9GAG website scraping method (preferred). \sa NineGagRequest
        InfiniGagSource //!< Use [InfiniGAG](https://github.com/k3min/infinigag) API. \sa InfiniGagRequest
    };

    explicit AppSettings(QObject *parent = 0);

    bool isWhiteTheme() const;
    void setWhiteTheme(bool whiteTheme);

    Source source() const;
    void setSource(Source source);

    bool scrollWithVolumeKeys() const;
    void setScrollWithVolumeKeys(bool scrollWithVolumeKeys);

    QStringList sections() const;
    void setSections(const QStringList &sections);

signals:
    void whiteThemeChanged();
    void sourceChanged();
    void scrollWithVolumeKeysChanged();
    void sectionsChanged();

private:
    Q_DISABLE_COPY(AppSettings)

    QSettings *m_settings;
    bool m_whiteTheme;
    Source m_source;
    bool m_scrollWithVolumeKeys;
    QStringList m_sections;
};

#endif // APPSETTINGS_H
