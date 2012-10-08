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

import QtQuick 1.1
import com.nokia.symbian 1.1

Item{
    id: root

    property string text: ""
    property alias checked: switchItem.checked

    width: parent.width
    height: switchItem.height + 2 * switchItem.anchors.topMargin

    Text{
        anchors{ left: parent.left; right: switchItem.left; verticalCenter: parent.verticalCenter }
        anchors.margins: constant.paddingMedium
        color: constant.colorLight
        font.pixelSize: constant.fontSizeMedium
        wrapMode: Text.Wrap
        text: root.text
    }

    Switch{
        id: switchItem
        anchors{ right: parent.right; top: parent.top; margins: constant.paddingMedium }
        platformInverted: settings.whiteTheme
        enabled: root.enabled
    }
}
