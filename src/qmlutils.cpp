/*
    Copyright (C) 2012  Dickson Leong
    This file is part of GagBook.

    GagBook is free software: you can redistribute it and/or modify it
    under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. If not, see http://www.gnu.org/licenses/.
*/

#include "qmlutils.h"

#include <QtDeclarative/QDeclarativeItem>
#include <QtCore/QSettings>
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
    QObject(parent), settings(new QSettings(this))
{
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

    if(!saved){
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
