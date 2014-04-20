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

#include "gagcookiejar.h"

#include <QtCore/QSettings>
#include <QtNetwork/QNetworkCookie>

GagCookieJar::GagCookieJar(QObject *parent) :
    QNetworkCookieJar(parent)
{
    QSettings settings;
    const QByteArray rawCookies = settings.value("cookies").toByteArray();
    setAllCookies(QNetworkCookie::parseCookies(rawCookies));
}

GagCookieJar::~GagCookieJar()
{
    QByteArray rawCookies;
    foreach (const QNetworkCookie cookie, allCookies()) {
        if (cookie.isSessionCookie())
            continue;

        if (!cookie.domain().contains("9gag.com"))
            continue;

        rawCookies += cookie.toRawForm() + "\n";
    }
    rawCookies.chop(1); // chop the last extra "\n"

    QSettings settings;
    settings.setValue("cookies", rawCookies);
}

void GagCookieJar::clear()
{
    setAllCookies(QList<QNetworkCookie>());
    QSettings settings;
    settings.remove("cookies");
}
