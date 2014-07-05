/*
 * Copyright (c) 2014 Dickson Leong.
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
import GagBook 1.0

Page {
    id: mainPage

    tools: ToolBarLayout {
        ToolIcon {
            platformIconId: "toolbar-back-dimmed"
            enabled: false
        }
        ToolIcon {
            platformIconId: "toolbar-list"
            onClicked: dialogManager.createSectionDialog()
        }
        ToolIcon {
            platformIconId: gagModel.busy ? "toolbar-stop" : "toolbar-refresh"
            onClicked: {
                if (gagModel.busy)
                    gagModel.stopRefresh();
                else
                    gagModel.refresh(GagModel.RefreshAll)
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
                text: "App Settings"
                onClicked: pageStack.push(Qt.resolvedUrl("AppSettingsPage.qml"))
            }
            MenuItem {
                text: "About GagBook"
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }
    }

    ListView {
        id: gagListView
        anchors { top: pageHeader.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
        model: gagModel
        orientation: ListView.Vertical
        delegate: GagDelegate {}
        footer: Item {
            width: ListView.view.width
            height: ListView.view.count > 0 ? footerColumn.height + 2 * constant.paddingLarge
                                            : ListView.view.height
            visible: gagModel.busy

            Column {
                id: footerColumn
                anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter }
                height: childrenRect.height
                spacing: constant.paddingMedium

                Text {
                    anchors { left: parent.left; right: parent.right }
                    horizontalAlignment: Text.AlignHCenter
                    elide: Text.ElideRight
                    font.pixelSize: constant.fontSizeMedium
                    color: constant.colorLight
                    text: "Downloading..."
                }

                ProgressBar {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width * 0.75
                    value: gagModel.progress
                    indeterminate: gagModel.progress == 0
                }
            }
        }

        onAtYEndChanged: if (atYEnd && !gagModel.busy && count > 0) gagModel.refresh(GagModel.RefreshOlder)
    }

    PageHeader {
        id: pageHeader
        anchors { top: parent.top; left: parent.left; right: parent.right }
        text: "/" + appSettings.sections[gagModel.selectedSection]
        busy: gagModel.busy
        onClicked: gagListView.positionViewAtBeginning()
    }

    GagModel {
        id: gagModel
        manager: gagbookManager
        onRefreshFailure: infoBanner.alert(errorMessage);
    }

    Connections {
        target: volumeKeyListener
        onVolumeUpClicked: {
            var currentIndex = gagListView.indexAt(gagListView.contentX, gagListView.contentY);
            if (currentIndex > 0)
                gagListView.positionViewAtIndex(currentIndex - 1, ListView.Beginning);
            else if (currentIndex == 0 && !gagListView.atYBeginning)
                gagListView.positionViewAtBeginning();
        }
        onVolumeDownClicked: {
            var currentIndex = gagListView.indexAt(gagListView.contentX, gagListView.contentY);
            if (currentIndex < gagListView.count - 1)
                gagListView.positionViewAtIndex(currentIndex + 1, ListView.Beginning);
            else if (currentIndex == gagListView.count - 1 && !gagListView.atYEnd)
                gagListView.positionViewAtEnd();
        }
    }

    Binding {
        target: volumeKeyListener
        property: "enabled"
        value: appSettings.scrollWithVolumeKeys && Qt.application.active && mainPage.status == PageStatus.Active
    }

    QtObject {
        id: dialogManager

        property Component __listModelComponent: Component { ListModel {} }
        property Component __selectionDialogComponent: Component { SelectionDialog {} }
        property Component __openLinkDialogComponent: null

        function createSectionDialog() {
            // convert array (appSettings.sections) to ListModel because SelectionDialog can not accept array
            var listModel = __listModelComponent.createObject(null);
            appSettings.sections.forEach(function(s) { listModel.append({ "text": s }) });

            var p = { titleText: "Section", model: listModel, selectedIndex: gagModel.selectedSection }
            var dialog = __selectionDialogComponent.createObject(mainPage, p);
            dialog.statusChanged.connect(function() {
                if (dialog.status == DialogStatus.Closed) {
                    dialog.destroy(250);
                    listModel.destroy();
                }
            });
            dialog.accepted.connect(function() {
                gagModel.selectedSection = dialog.selectedIndex;
                gagModel.refresh(GagModel.RefreshAll);
            });
            dialog.open();
        }

        function createOpenLinkDialog(link) {
            if (!__openLinkDialogComponent)
                __openLinkDialogComponent = Qt.createComponent("OpenLinkDialog.qml")
            var dialog = __openLinkDialogComponent.createObject(mainPage, { link: link });
            dialog.statusChanged.connect(function() {
                if (dialog.status == DialogStatus.Closed)
                    dialog.destroy(250);
            });
            dialog.open();
        }
    }
}
