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

#ifndef QMLUTILS_H
#define QMLUTILS_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

class QDeclarativeItem;
class QClipboard;
class QSettings;

class QMLUtils : public QObject
{
    Q_OBJECT
public:
    explicit QMLUtils(QObject *parent = 0);

    // Copy text to system clipboard
    Q_INVOKABLE void copyToClipboard(const QString &text);

    // Save a image from QML Image object
    Q_INVOKABLE QString saveImage(QDeclarativeItem *imagObject, const int id);

    // QSettings
    Q_INVOKABLE QVariant getSetting(const QString &key, const QVariant &defaultValue = QVariant());
    Q_INVOKABLE void setSetting(const QString &key, const QVariant &value);

private:
    Q_DISABLE_COPY(QMLUtils)

    QClipboard *clipboard;
    QSettings *settings;
};

#endif // QMLUTILS_H
