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
    property variant buttonsText: []
    property int checkedButtonIndex: 0

    signal buttonClicked(int index)

    width: parent.width
    height: settingText.paintedHeight + buttonRow.height + buttonRow.anchors.topMargin

    Text{
        id: settingText
        anchors{ left: parent.left; top: parent.top; leftMargin: constant.paddingMedium }
        font.pixelSize: constant.fontSizeMedium
        color: constant.colorLight
        text: root.text
    }

    ButtonRow{
        id: buttonRow
        anchors{
            top: settingText.bottom
            margins: constant.paddingMedium
            left: parent.left
            right: parent.right
        }
        checkedButton: checkedButtonIndex === 0 ? firstButton : secondButton
        onVisibleChanged: checkedButtonIndex === 0 ? firstButton : secondButton

        // Stupid ButtonRow in Symbian component doesn't support using repeater to dynamically create buttons
        // So I have to create button statically
        Button{
            id: firstButton
            platformInverted: settings.whiteTheme
            text: root.buttonsText[0]
            onClicked: root.buttonClicked(0)
        }

        Button{
            id: secondButton
            platformInverted: settings.whiteTheme
            text: root.buttonsText[1]
            onClicked: root.buttonClicked(1)
        }
    }
}
