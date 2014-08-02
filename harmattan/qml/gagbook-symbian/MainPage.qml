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
import com.nokia.symbian 1.1
import GagBook.Core 1.0

Page {
    id: mainPage

    tools: ToolBarLayout {
        ToolButton {
            platformInverted: appSettings.whiteTheme
            iconSource: "Images/close_stop" + (platformInverted ? "_inverted.svg" : ".svg")
            onClicked: Qt.quit()
        }
        ToolButton {
            platformInverted: appSettings.whiteTheme
            iconSource: "toolbar-list"
            onClicked: dialogManager.createSectionDialog()
        }
        ToolButton {
            platformInverted: appSettings.whiteTheme
            iconSource: gagModel.busy ? ("Images/close_stop" + (platformInverted ? "_inverted.svg" : ".svg"))
                                        : "toolbar-refresh"
            onClicked: {
                if (gagModel.busy)
                    gagModel.stopRefresh()
                else
                    gagModel.refresh(GagModel.RefreshAll)
            }
        }
        ToolButton {
            platformInverted: appSettings.whiteTheme
            iconSource: "toolbar-menu"
            onClicked: mainMenu.open()
        }
    }

    Menu {
        id: mainMenu
        platformInverted: appSettings.whiteTheme

        MenuLayout {
            MenuItem {
                platformInverted: appSettings.whiteTheme
                text: "App Settings"
                onClicked: pageStack.push(Qt.resolvedUrl("AppSettingsPage.qml"))
            }
            MenuItem {
                platformInverted: appSettings.whiteTheme
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
            height: footerLoader.status == Loader.Ready ? footerLoader.height + 2 * footerLoader.anchors.margins : 0

            // show downloading indicator at the end of the list
            Loader {
                id: footerLoader
                anchors { left: parent.left; right: parent.right; top: parent.top; margins: constant.paddingLarge  }
                sourceComponent: gagModel.busy && gagListView.count > 0 ? downloadingIndicator : undefined
            }
        }

        // show downloading indicator when the list is empty
        Loader {
            anchors {
                left: parent.left; right: parent.right
                verticalCenter: parent.verticalCenter
                margins: constant.paddingLarge
            }
            sourceComponent: gagModel.busy && gagListView.count == 0 ? downloadingIndicator : undefined

            Component {
                id: downloadingIndicator

                Column {
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
                        platformInverted: appSettings.whiteTheme
                        width: parent.width * 0.75
                        value: gagModel.progress
                        // when indeterminate change from true to false the indeterminate
                        // graphic is still there (bug?)
                        //indeterminate: gagModel.progress == 0
                    }
                }
            }
        }

        onAtYEndChanged: if (atYEnd && !gagModel.busy && count > 0) gagModel.refresh(GagModel.RefreshOlder)
    }

    PageHeader {
        id: pageHeader
        anchors { top: parent.top; left: parent.left; right: parent.right }
        text: "/" + appSettings.sections[gagModel.selectedSection]
        busy: gagModel.busy || votingManager.busy
        onClicked: gagListView.positionViewAtBeginning()
    }

    GagModel {
        id: gagModel
        manager: gagbookManager
        onRefreshFailure: infoBanner.alert(errorMessage);
    }

    VotingManager {
        id: votingManager
        manager: gagbookManager
        onVoteSuccess: gagModel.changeLikes(id, likes);
        onFailure: infoBanner.alert(errorMessage);
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

        property Component __selectionDialogComponent: Component { SelectionDialog {} }
        property Component __openLinkDialogComponent: null
        property Component __shareDialogComponent: null

        function createSectionDialog() {
            var p = { platformInverted: appSettings.whiteTheme, titleText: "Section",
                model: appSettings.sections, selectedIndex: gagModel.selectedSection }
            var dialog = __selectionDialogComponent.createObject(mainPage, p);
            dialog.statusChanged.connect(function() {
                if (dialog.status == DialogStatus.Closed)
                    dialog.destroy(250);
            });
            dialog.accepted.connect(function() {
                gagModel.selectedSection = dialog.selectedIndex;
                gagModel.refresh(GagModel.RefreshAll)
            })
            dialog.open();
        }

        function createOpenLinkDialog(link) {
            if (!__openLinkDialogComponent)
                __openLinkDialogComponent = Qt.createComponent("OpenLinkDialog.qml");
            var dialog = __openLinkDialogComponent.createObject(mainPage, { link: link });
            dialog.statusChanged.connect(function() {
                if (dialog.status == DialogStatus.Closed)
                    dialog.destroy(250);
            });
            dialog.open();
        }

        function createShareDialog(link) {
            if (!__shareDialogComponent)
                __shareDialogComponent = Qt.createComponent("ShareDialog.qml");
            var dialog = __shareDialogComponent.createObject(mainPage, { link: link });
            dialog.statusChanged.connect(function() {
                if (dialog.status == DialogStatus.Closed)
                    dialog.destroy(250);
            });
            dialog.open();
        }
    }
}
