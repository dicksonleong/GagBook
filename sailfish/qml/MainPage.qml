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

import QtQuick 2.0
import Sailfish.Silica 1.0
import GagBook 1.0

Page {
    id: mainPage

    SilicaListView {
        id: gagListView
        anchors.fill: parent
        model: gagModel
        orientation: ListView.Vertical
        header: PageHeader {title: "/" + appSettings.sections[gagModel.selectedSection]}

        ViewPlaceholder {
            id: placeHolder
            text: "Couldn't get any gags"
            enabled: gagListView.count == 0 && !gagModel.busy
            hintText: qsTr("Check your WiFi or 3G settings and" +
                           "\ntry refreshing from the menu. ")
        }

        delegate: GagDelegate {}
        footer: Item {
            //anchors.centerIn: parent
            width: parent.width
            height: gagModel.count > 0 ? 200 : ListView.view.height
            visible: gagModel.busy

            BusyIndicator {
                id: partialProgressBar
                anchors.margins: Theme.paddingLarge
                anchors.centerIn: parent
                size: gagModel.count > 0 ? BusyIndicatorSize.Medium : BusyIndicatorSize.Large
                running: gagModel.busy
                //value: gagModel.progress
                //indeterminate: gagModel.progress == 0
            }

        }

        onAtYEndChanged: if (atYEnd && !gagModel.busy && count > 0) gagModel.refresh(GagModel.RefreshOlder)

        PullDownMenu {
            MenuItem {
                text: "About GagBook"
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                text: "More sections..."
                onClicked: dialogManager.createSectionDialog()
            }
            MenuItem {
                text: gagModel.selectedSection == 0 ? "Trending" : "Hot"
                onClicked: {
                    gagModel.selectedSection = gagModel.selectedSection == 0 ? 1:0;
                    gagModel.refresh(GagModel.RefreshAll);
                }
            }
            MenuItem {
                text: "Refresh"
                onClicked: {
                    gagModel.refresh(GagModel.RefreshAll);
                }
            }
        }
    }
    GagModel {
        id: gagModel
        manager: gagbookManager
        onRefreshFailure: {
            console.log("refresh failed");
            state = "noConnection";
        }
        onBusyChanged: {
            console.log("working...");
        }
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

            var p = { title: "Section", model: listModel, selectedIndex: gagModel.selectedSection }
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
