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

#include "qmlimagesaver.h"

#include <QImage>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDesktopServices>
#include <QDebug>

QMLImageSaver::QMLImageSaver(QObject *parent) :
    QObject(parent)
{
}

QString QMLImageSaver::save(QDeclarativeItem *imageObject, const int id)
{
    QString fileName = "gagbook_" + QString::number(id) + ".jpg";

    // NOTE: QDesktopServices::storageLocation(QDesktopServices::PicturesLocation)
    // may return /home/user/ (?) if the tracker is screwed up in the OS. (Tracker bug)
    // May need to hardcode the path /home/user/MyDocs/Picture if this bug is too common
    // If you have such problem, check the file /home/user/.config/user-dirs.dirs
    QString savingFilePath = QDesktopServices::storageLocation(QDesktopServices::PicturesLocation) + "/" + fileName;

    QImage img(imageObject->boundingRect().size().toSize(), QImage::Format_RGB32);
    img.fill(QColor(255,255,255).rgb());
    QPainter painter(&img);
    QStyleOptionGraphicsItem styleOption;
    imageObject->paint(&painter, &styleOption, 0);
    bool saved = img.save(savingFilePath, "JPG");

    if(!saved){
        qDebug() << "QMLImageSaver::save Failed to save image to" << savingFilePath ;
        return "";
    }

    return savingFilePath;
}
