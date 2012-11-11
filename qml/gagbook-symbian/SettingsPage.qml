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

Page{
    id: settingsPage

    tools: ToolBarLayout{
        ToolButton{
            platformInverted: settings.whiteTheme
            iconSource: "toolbar-back"
            onClicked: pageStack.pop()
        }
    }

    Column{
        id: settingsColumn
        anchors{ left: parent.left; right: parent.right; top: pageHeader.bottom; topMargin: constant.paddingMedium }
        height: childrenRect.height
        spacing: constant.paddingMedium

        SettingButtonRow{
            text: "White Theme"
            checkedButtonIndex: settings.whiteTheme ? 1 : 0
            buttonsText: ["Dark", "White"]
            onButtonClicked: settings.whiteTheme = index === 1
        }

        SettingButtonRow{
            text: "Image Size"
            checkedButtonIndex: settings.imageSize
            buttonsText: ["Medium", "Large"]
            onButtonClicked: settings.imageSize = index
        }
    }

    PageHeader{
        id: pageHeader
        text: "Settings"
    }
}
