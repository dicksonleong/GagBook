/*
 * Copyright (c) 2012-2013 Dickson Leong.
 * All rights reserved.
 *
 * This file is part of GagBook.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of GagBook nor the names of its contributors may be
 *       used to endorse or promote products derived from this software without
 *       specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

import QtQuick 1.1
import com.nokia.meego 1.0
import "MainPage.js" as Script
import GagBook 1.0

Page {
    id: mainPage

    tools: ToolBarLayout {
        ToolIcon {
            platformIconId: "toolbar-back" + (enabled ? "" : "-dimmed")
            enabled: gagListView.count > 0 && gagListView.currentItem.imageZoomed
            onClicked: gagListView.currentItem.resetImageZoom()
        }
        ToolIcon {
            platformIconId: "toolbar-new-message" + (enabled ? "" : "-dimmed")
            enabled: gagListView.count > 0
            onClicked: {
                var prop = {gagURL: gagListView.model.get(gagListView.currentIndex).url}
                pageStack.push(Qt.resolvedUrl("CommentsPage.qml"), prop)
            }
        }
        ToolIcon {
            iconSource: "image://theme/icon-l-browser-main-view"
            enabled: gagListView.count > 0
            opacity: enabled ? 1 : 0.25
            onClicked: Script.createOpenLinkDialog(gagListView.model.get(gagListView.currentIndex).url)
        }
        ToolIcon {
            platformIconId: "toolbar-share" + (enabled ? "" : "-dimmed")
            enabled: gagListView.count > 0
            onClicked: {
                var gag = gagListView.model.get(gagListView.currentIndex);
                QMLUtils.shareLink(gag.url, gag.title);
            }
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            onClicked: mainMenu.open()
        }
    }

    Menu {
        id: mainMenu

        MenuLayout {
            MenuItem {
                text: "Refresh section"
                enabled: !gagManager.busy
                onClicked: gagManager.refresh(GagManager.RefreshAll)
            }
            MenuItem {
                text: "Save image"
                enabled: gagListView.count > 0
                onClicked: {
                    var filePath = gagListView.currentItem.saveImage()
                    if (filePath) infoBanner.alert("Image saved in " + filePath)
                    else infoBanner.alert("Failed to save image")
                }
            }
            MenuItem {
                text: "Settings"
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
            MenuItem {
                text: "About GagBook"
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }
    }

    ListView {
        id: gagListView
        anchors { top: pageHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        model: gagManager.model
        boundsBehavior: Flickable.DragOverBounds
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        delegate: GagDelegate {}
        interactive: moving || count === 0 || !currentItem.allowDelegateFlicking

        onCurrentIndexChanged: {
            if ((currentIndex === count - 1) && currentIndex >= 0 && !gagManager.busy)
                gagManager.refresh(GagManager.RefreshOlder);
        }
        onAtXEndChanged: if (count > 0 && atXEnd && !gagManager.busy) gagManager.refresh(GagManager.RefreshOlder);
        onCurrentItemChanged: if (currentItem) currentItem.loadImage = true
    }

    PageHeader {
        id: pageHeader
        text: sectionModel.get(settings.selectedSection).text
        busy: gagManager.busy
        comboboxVisible: true
        onClicked: Script.createSectionDialog()
    }
}
