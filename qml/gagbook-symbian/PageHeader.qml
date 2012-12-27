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
import com.nokia.extras 1.1

Item {
    id: root

    property string text
    property bool busy: false
    property bool comboboxVisible: false
    signal clicked

    height: constant.headerHeight
    width: parent.width

    Image {
        id: background
        anchors.fill: parent
        sourceSize { width: parent.width; height: parent.height }
        source: headerPress.pressed ? "Images/color10-meegotouch-view-header-fixed-pressed.png"
                                    : "Images/color10-meegotouch-view-header-fixed.png"
    }

    Text {
        id: mainText
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: busyLoader.left
            margins: constant.paddingMedium
        }
        font.pixelSize: constant.fontSizeXLarge
        color: "white"
        elide: Text.ElideRight
        text: root.text
    }

    Loader {
        id: busyLoader
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right; rightMargin: constant.paddingMedium
        }
        sourceComponent: busy ? updatingIndicator : (comboboxVisible ? combobox : undefined)
    }

    Component {
        id: updatingIndicator

        BusyIndicator {
            platformInverted: false
            width: platformStyle.graphicSizeSmall; height: width
            running: true
        }
    }

    Component { id: combobox; Image { source: "Images/meegotouch-combobox-indicator-inverted.png" } }

    Image {
        anchors { top: parent.top; left: parent.left }
        source: "Images/meegoTLCorner.png"
    }

    Image {
        anchors { top: parent.top; right: parent.right }
        source: "Images/meegoTRCorner.png"
    }

    MouseArea {
        id: headerPress
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
