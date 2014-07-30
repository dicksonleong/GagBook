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

#ifndef QMLUTILS_H
#define QMLUTILS_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QScopedPointer>
#include <QtCore/QUrl>

/*! Utilities functions for QML

    Collection of utilities functions and constant properties for use by QML
 */
class QMLUtils : public QObject
{
    Q_OBJECT
    /*! The maximum height for a image to show properly. Value is platform dependent. */
    Q_PROPERTY(int IMAGE_MAX_HEIGHT READ imageMaxHeight CONSTANT)
    /*! The url of the GagBook repository. */
    Q_PROPERTY(QUrl REPO_WEBSITE READ repoWebsite CONSTANT)
public:
    explicit QMLUtils(QObject *parent = 0);

    int imageMaxHeight() const;
    QUrl repoWebsite() const;

    /*! Copy the \p text to system clipboard. */
    Q_INVOKABLE void copyToClipboard(const QString &text);

    /*! Save a cached image to gallery. The \p imageUrl must be a url point to
      a **local** cached image file. */
    Q_INVOKABLE QString saveImage(const QUrl &imageUrl);

    /*! Share the \p link using Harmattan Share UI. Only available for Harmattan. */
    Q_INVOKABLE void shareLink(const QString &link, const QString &title = QString());

    /*! Open the \p url using Symbian's default browser. Qt.openUrlExternally() in QML
        does not open the default browser in Symbian and this function is a workaround.
        On other platform, please use Qt.openUrlExternally(). */
    Q_INVOKABLE void openDefaultBrowser(const QUrl &url);

private:
    Q_DISABLE_COPY(QMLUtils)
};

#endif // QMLUTILS_H
