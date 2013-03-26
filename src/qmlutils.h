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
 *     * Neither the name of GagBook nor the names of its contributors may be
 *       used to endorse or promote products derived from this software without
 *       specific prior written permission.
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

#ifndef QMLUTILS_H
#define QMLUTILS_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QScopedPointer>

class QDeclarativeItem;
class QUrl;

class QMLUtils : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int IMAGE_MAX_HEIGHT READ imageMaxHeight CONSTANT)
    Q_PROPERTY(QString dataDownloaded READ dataDownloaded NOTIFY dataDownloadedChanged)
public:
    static QMLUtils *instance();

    static const int IMAGE_MAX_HEIGHT;
    int imageMaxHeight() const { return IMAGE_MAX_HEIGHT; }

    // Copy text to system clipboard
    Q_INVOKABLE void copyToClipboard(const QString &text);

    // Save an image
    Q_INVOKABLE QString saveImage(const QUrl &imageUrl);

    // Share a link using Harmattan Share UI
    Q_INVOKABLE void shareLink(const QString &link, const QString &title = QString());

    // Open the link using Symbian's default browser
    Q_INVOKABLE void openDefaultBrowser(const QUrl &url);

    QString dataDownloaded() const { return m_dataDownloadedStr; }
    void increaseDataDownloaded(qint64 bytesDownloaded);

signals:
    void dataDownloadedChanged();

private:
    static QScopedPointer<QMLUtils> m_instance;

    explicit QMLUtils(QObject *parent = 0);
    Q_DISABLE_COPY(QMLUtils)

    qint64 m_dataDownloaded; // in bytes
    QString m_dataDownloadedStr; // in MB
};

#endif // QMLUTILS_H
