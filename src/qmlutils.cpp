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

#include "qmlutils.h"

#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtCore/QSettings>
#include <QtDeclarative/QDeclarativeItem>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QDesktopServices>

namespace {
    // NOTE: QDesktopServices::storageLocation(QDesktopServices::PicturesLocation) in Harmattan
    // may return /home/user/ (?) due to a bug in the tracker.
    // If you have such problem, check the file /home/user/.config/user-dirs.dirs
    const QString SAVING_FILE_PATH = QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
}

QMLUtils::QMLUtils(QObject *parent) :
    QObject(parent), clipboard(QApplication::clipboard()), settings(new QSettings(this))
{
}

void QMLUtils::copyToClipboard(const QString &text)
{
    clipboard->setText(text, QClipboard::Clipboard);
    clipboard->setText(text, QClipboard::Selection);
}

QString QMLUtils::saveImage(QDeclarativeItem *imageObject, const int id)
{
    QString fileName = "gagbook_" + QString::number(id) + ".jpg";
    QString filePath = SAVING_FILE_PATH + "/" + fileName;

    QImage img(imageObject->boundingRect().size().toSize(), QImage::Format_RGB32);
    img.fill(QColor(255,255,255).rgb());
    QPainter painter(&img);
    QStyleOptionGraphicsItem styleOption;
    imageObject->paint(&painter, &styleOption, 0);
    bool saved = img.save(filePath, "JPG");

    if (!saved) {
        qWarning("QMLUtils::saveImage: Unable to save image to %s", qPrintable(filePath));
        return "";
    }

    return filePath;
}

QVariant QMLUtils::getSetting(const QString &key, const QVariant &defaultValue)
{
    return settings->value(key, defaultValue);
}

void QMLUtils::setSetting(const QString &key, const QVariant &value)
{
    settings->setValue(key, value);
}
