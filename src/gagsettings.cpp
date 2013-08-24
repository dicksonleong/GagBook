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

#include "gagsettings.h"

#include <QtCore/QSettings>

QScopedPointer<GagSettings> GagSettings::m_instance(0);

GagSettings *GagSettings::instance()
{
    if (m_instance.isNull())
        m_instance.reset(new GagSettings);

    return m_instance.data();
}

int GagSettings::selectedSection() const
{
    return m_selectedSection;
}

void GagSettings::setSelectedSection(int selectedSection)
{
    if (m_selectedSection != selectedSection) {
        m_selectedSection = selectedSection;
        m_settings->setValue("selectedSection", m_selectedSection);
        emit selectedSectionChanged();
    }
}

bool GagSettings::isWhiteTheme() const
{
    return m_whiteTheme;
}

void GagSettings::setWhiteTheme(bool whiteTheme)
{
    if (m_whiteTheme != whiteTheme) {
        m_whiteTheme = whiteTheme;
        m_settings->setValue("whiteTheme", m_whiteTheme);
        emit whiteThemeChanged();
    }
}

int GagSettings::source() const
{
    return m_source;
}

void GagSettings::setSource(int source)
{
    if (m_source != source) {
        m_source = source;
        m_settings->setValue("source", m_source);
        emit sourceChanged();
    }
}

int GagSettings::autoDownloadGif() const
{
    return m_autoDownloadGif;
}

void GagSettings::setAutoDownloadGif(int autoDownloadGif)
{
    if (m_autoDownloadGif != autoDownloadGif) {
        m_autoDownloadGif = autoDownloadGif;
        m_settings->setValue("autoDownloadGif", m_autoDownloadGif);
        emit autoDownloadGifChanged();
    }
}

GagSettings::GagSettings(QObject *parent) :
    QObject(parent), m_settings(new QSettings("GagBook", "GagBook", this))
{
    m_selectedSection = m_settings->value("selectedSection", 0).toInt();
    m_whiteTheme = m_settings->value("whiteTheme", false).toBool();
    m_source = m_settings->value("source", 0).toInt();
    m_autoDownloadGif = m_settings->value("autoDownloadGif", 0).toInt();
}
