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

GagSettings::GagSettings(QObject *parent) :
    QObject(parent), m_settings(new QSettings(this))
{
    m_whiteTheme = m_settings->value("whiteTheme", false).toBool();
    m_section = static_cast<Section>(m_settings->value("section", 0).toInt());
    m_source = static_cast<Source>(m_settings->value("source", 0).toInt());
    m_gifDownloadMode = static_cast<GifDownloadMode>(m_settings->value("gifDownloadMode", 0).toInt());
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

GagSettings::Section GagSettings::section() const
{
    return m_section;
}

void GagSettings::setSection(Section section)
{
    if (m_section != section) {
        m_section = section;
        m_settings->setValue("section", static_cast<int>(m_section));
        emit sectionChanged();
    }
}

GagSettings::Source GagSettings::source() const
{
    return m_source;
}

void GagSettings::setSource(Source source)
{
    if (m_source != source) {
        m_source = source;
        m_settings->setValue("source", static_cast<int>(m_source));
        emit sourceChanged();
    }
}

GagSettings::GifDownloadMode GagSettings::gifDownloadMode() const
{
    return m_gifDownloadMode;
}

void GagSettings::setGifDownloadMode(GifDownloadMode mode)
{
    if (m_gifDownloadMode != mode) {
        m_gifDownloadMode = mode;
        m_settings->setValue("gifDownloadMode", static_cast<int>(m_gifDownloadMode));
        emit gifDownloadModeChanged();
    }
}
