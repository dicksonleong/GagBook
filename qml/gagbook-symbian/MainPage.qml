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
import "infinigag.js" as Server
import "MainPage.js" as Script

Page {
    id: mainPage

    property int nextPageId: 0

    tools: ToolBarLayout {
        ToolButton {
            property bool __canResetZoom: gagListView.count > 0 && gagListView.currentItem.imageZoomed
            platformInverted: settings.whiteTheme
            iconSource: __canResetZoom ? "toolbar-back"
                                       : ("Images/close_stop" + (platformInverted ? "_inverted.svg" : ".svg"))
            onClicked: __canResetZoom ? gagListView.currentItem.resetImageZoom() : Qt.quit()
        }
        ToolButton {
            platformInverted: settings.whiteTheme
            iconSource: "Images/instant_messenger_chat" + (platformInverted ? "_inverted.svg" : ".svg")
            enabled: gagListView.count > 0
            opacity: enabled ? 1 : 0.25
            onClicked: {
                var prop = {gagURL: gagListView.model.get(gagListView.currentIndex).url}
                pageStack.push(Qt.resolvedUrl("CommentsPage.qml"), prop)
            }
        }
        ToolButton {
            platformInverted: settings.whiteTheme
            iconSource: "Images/internet" + (platformInverted ? "_inverted.svg" : ".svg")
            enabled: gagListView.count > 0
            opacity: enabled ? 1 : 0.25
            onClicked: Script.createOpenLinkDialog(gagListView.model.get(gagListView.currentIndex).url)
        }
        ToolButton {
            platformInverted: settings.whiteTheme
            iconSource: "toolbar-share"
            enabled: gagListView.count > 0
            opacity: enabled ? 1 : 0.25
            onClicked: Script.createShareDialog(gagListView.model.get(gagListView.currentIndex).url)
        }
        ToolButton {
            platformInverted: settings.whiteTheme
            iconSource: "toolbar-menu"
            onClicked: mainMenu.open()
        }
    }

    Menu {
        id: mainMenu
        platformInverted: settings.whiteTheme

        MenuLayout {
            MenuItem {
                platformInverted: settings.whiteTheme
                text: "Refresh section"
                enabled: !pageHeader.busy
                onClicked: Script.refresh()
            }
            MenuItem {
                platformInverted: settings.whiteTheme
                text: "Save image"
                enabled: gagListView.count > 0
                onClicked: {
                    var filePath = gagListView.currentItem.saveImage()
                    if (filePath) infoBanner.alert("Image saved in " + filePath)
                    else infoBanner.alert("Failed to save image")
                }
            }
            MenuItem {
                platformInverted: settings.whiteTheme
                text: "Settings"
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
            MenuItem {
                platformInverted: settings.whiteTheme
                text: "About GagBook"
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }
    }

    ListView {
        id: gagListView
        anchors { top: pageHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        model: ListModel {}
        boundsBehavior: Flickable.DragOverBounds
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        delegate: GagDelegate {}
        interactive: moving || count === 0 || !currentItem.allowDelegateFlicking

        onAtXEndChanged: if (atXEnd && count > 0 && !pageHeader.busy) Script.refresh(false)
        onCurrentItemChanged: if (currentItem) currentItem.loadImage = true
    }

    PageHeader {
        id: pageHeader
        text: {
            switch (settings.selectedSection) {
            case 0: return "Hot"
            case 1: return "Trending"
            case 2: return "Vote"
            default: return ""
            }
        }
        comboboxVisible: true
        onClicked: Script.createSectionDialog()
    }

    Component.onCompleted: Script.refresh()
}
