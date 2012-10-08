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
import com.nokia.meego 1.0

Page{
    id: settingsPage

    tools: ToolBarLayout{
        ToolIcon{
            platformIconId: "toolbar-back"
            onClicked: pageStack.pop()
        }
    }

    Column{
        id: settingsColumn
        anchors{ left: parent.left; right: parent.right; top: pageHeader.bottom; topMargin: constant.paddingMedium }
        height: childrenRect.height
        spacing: constant.paddingMedium

        SettingSwitch{
            text: "White Theme"
            checked: settings.whiteTheme
            onCheckedChanged: settings.whiteTheme = checked
        }

        Item{
            width: parent.width
            height: imageSizeText.paintedHeight + imageSizeButtonRow.height + imageSizeButtonRow.anchors.topMargin

            Text{
                id: imageSizeText
                anchors{ left: parent.left; top: parent.top; leftMargin: constant.paddingMedium }
                font.pixelSize: constant.fontSizeMedium
                color: constant.colorLight
                text: "Image Size"
            }

            ButtonRow{
                id: imageSizeButtonRow
                anchors{
                    top: imageSizeText.bottom
                    margins: constant.paddingMedium
                    left: parent.left
                    right: parent.right
                }
                checkedButton: settings.imageSize === 0 ? mediumImageSizeButton : largeImageSizeButton
                onCheckedButtonChanged: {
                    if(settingsPage.status === PageStatus.Active)
                        settings.imageSize = (checkedButton === mediumImageSizeButton ? 0 : 1)
                }

                Button{
                    id: mediumImageSizeButton
                    text: "Medium"
                }
                Button{
                    id: largeImageSizeButton
                    text: "Large"
                }
            }
        }
    }

    PageHeader{
        id: pageHeader
        text: "Settings"
    }
}
