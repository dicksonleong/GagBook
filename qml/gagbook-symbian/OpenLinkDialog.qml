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

ContextMenu{
    id: root

    property string link

    property bool __isClosing: false

    platformInverted: settings.whiteTheme

    MenuLayout{
        Text{
            anchors{ left: parent.left; right: parent.right }
            horizontalAlignment: Text.AlignHCenter
            text: link
            font.italic: true
            font.pixelSize: constant.fontSizeMedium
            color: constant.colorLight
            elide: Text.ElideRight
            maximumLineCount: 3
            wrapMode: Text.WrapAnywhere
        }
        MenuItem{
            text: "Open link in web browser"
            platformInverted: root.platformInverted
            onClicked: {
                Qt.openUrlExternally(link)
                infoBanner.alert("Launching web browser...")
            }
        }
        MenuItem{
            text: "Copy link"
            platformInverted: root.platformInverted
            onClicked: {
                QMLUtils.copyToClipboard(link)
                infoBanner.alert("Link copied to clipboard")
            }
        }
    }

    Component.onCompleted: {
        console.log("Dialog created:", root)
        open()
    }
    Component.onDestruction: console.log("Dialog destructing:", root)

    onStatusChanged: {
        if(status === DialogStatus.Closing) __isClosing = true
        else if(status === DialogStatus.Closed && __isClosing) root.destroy(250)
    }
}
