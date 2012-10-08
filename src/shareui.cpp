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

#include "shareui.h"

#ifdef Q_OS_HARMATTAN
#include <MDataUri>
#include <maemo-meegotouch-interfaces/shareuiinterface.h>
#endif

ShareUI::ShareUI(QObject *parent) :
    QObject(parent)
{
}

void ShareUI::shareLink(const QString &link, const QString &title)
{
#ifdef Q_OS_HARMATTAN
    MDataUri uri;
    uri.setMimeType("text/x-url");
    uri.setTextData(link);

    if(!title.isEmpty()){
        uri.setAttribute("title", title);
    }

    if(!uri.isValid()){
        qCritical("Invalid URI");
        return;
    }

    ShareUiInterface shareIf("com.nokia.ShareUi");

    if(!shareIf.isValid()){
        qCritical("Invalid Share UI interface");
        return;
    }

    shareIf.share(QStringList() << uri.toString());
#else
    Q_UNUSED(title)
    Q_UNUSED(link)
#endif
}
