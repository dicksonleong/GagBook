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

ContextMenu {
    id: root

    property string link

    property bool __isClosing: false

    platformInverted: settings.whiteTheme

    MenuLayout {
        MenuItem {
            text: "Share via Facebook"
            platformInverted: root.platformInverted
            onClicked: {
                Qt.openUrlExternally("http://www.facebook.com/sharer.php?u="+link)
                infoBanner.alert("Launching web browser...")
            }
        }
        MenuItem {
            text: "Share via SMS"
            platformInverted: root.platformInverted
            onClicked: {
                Qt.openUrlExternally("sms:?body="+link)
                infoBanner.alert("Launching SMS...")
            }
        }
        MenuItem {
            text: "Share via email"
            platformInverted: root.platformInverted
            onClicked: {
                Qt.openUrlExternally("mailto:?body="+link)
                infoBanner.alert("Launching email client...")
            }
        }
    }

    Component.onCompleted: {
        console.log("Dialog created:", root)
        open()
    }
    Component.onDestruction: console.log("Dialog destructing:", root)

    onStatusChanged: {
        if (status === DialogStatus.Closing) __isClosing = true
        else if (status === DialogStatus.Closed && __isClosing) root.destroy(250)
    }
}
