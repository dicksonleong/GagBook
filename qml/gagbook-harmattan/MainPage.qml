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
import "infinigag.js" as Server
import "MainPage.js" as Script

Page {
    id: mainPage

    property int nextPageId: 0

    tools: ToolBarLayout{
        ToolIcon{
            platformIconId: "toolbar-back" + (enabled ? "" : "-dimmed")
            enabled: gagListView.count > 0 && gagListView.currentItem.imageZoomed
            onClicked: gagListView.currentItem.resetImageZoom()
        }
        ToolIcon{
            platformIconId: "toolbar-new-message" + (enabled ? "" : "-dimmed")
            enabled: gagListView.count > 0
            onClicked: {
                var prop = {gagURL: gagListView.model.get(gagListView.currentIndex).url}
                pageStack.push(Qt.resolvedUrl("CommentsPage.qml"), prop)
            }
        }
        ToolIcon{
            // FIXME: Better toolbar icon for open link
            iconSource: "image://theme/icon-l-browser-main-view"
            enabled: gagListView.count > 0
            opacity: enabled ? 1 : 0.25
            onClicked: Script.createOpenLinkQueryDialog(gagListView.model.get(gagListView.currentIndex).url)
        }
        ToolIcon{
            platformIconId: "toolbar-share" + (enabled ? "" : "-dimmed")
            enabled: gagListView.count > 0
            onClicked: shareUI.shareLink(gagListView.model.get(gagListView.currentIndex).url,
                                         gagListView.model.get(gagListView.currentIndex).title)
        }
        ToolIcon{
            platformIconId: "toolbar-view-menu"
            onClicked: mainMenu.open()
        }
    }

    Menu{
        id: mainMenu

        MenuLayout{
            MenuItem{
                text: "Refresh section"
                enabled: !pageHeader.busy
                onClicked: Script.refresh()
            }
            MenuItem{
                text: "Save image"
                enabled: gagListView.count > 0
                onClicked: {
                    var filePath = gagListView.currentItem.saveImage()
                    if(filePath) infoBanner.alert("Image saved in " + filePath)
                    else infoBanner.alert("Failed to save image")
                }
            }
            MenuItem{
                text: "Settings"
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
            MenuItem{
                text: "About GagBook"
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }
    }

    ListView{
        id: gagListView
        anchors{ top: pageHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        model: ListModel{}
        boundsBehavior: Flickable.DragOverBounds
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        delegate: GagDelegate{}
        interactive: moving || count === 0 || !currentItem.allowDelegateFlicking

        onAtXEndChanged: if(atXEnd && count > 0 && !pageHeader.busy) Script.refresh(false)
        onCurrentItemChanged: if(currentItem) currentItem.loadImage = true
    }

    PageHeader{
        id: pageHeader
        text: {
            switch(settings.selectedSection){
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
