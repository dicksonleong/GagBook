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

#ifndef QMLIMAGESAVER_H
#define QMLIMAGESAVER_H

#include <QDeclarativeItem>

class QMLImageSaver : public QObject
{
    Q_OBJECT
public:
    explicit QMLImageSaver(QObject *parent = 0);

    Q_INVOKABLE QString save(QDeclarativeItem *imageObject, const int id);
};

#endif // QMLIMAGESAVER_H
