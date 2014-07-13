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
import harbour.gagbook.Core 1.0

Page {
    id: mainPage
    objectName: "mainPage"

    // for access by cover
    property alias gagModel: gagModel
    property int currentIndex: 0

    function positionAtIndex(index) {
        gagListView.positionViewAtIndex(index, ListView.Beginning);
        currentIndex = index;
    }

    property bool __pushedAttached: false
    onStatusChanged: {
        if (status == PageStatus.Active && !__pushedAttached) {
            pageStack.pushAttached(Qt.resolvedUrl("SectionPage.qml"), { gagModel: gagModel });
            __pushedAttached = true;
        }
    }

    SilicaListView {
        id: gagListView
        anchors.fill: parent
        model: gagModel
        orientation: ListView.Vertical
        spacing: constant.paddingMedium
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
            id: footerItem
            width: parent.width
            height: {
                if (gagListView.count === 0)
                    return gagListView.height - (gagListView.headerItem ? gagListView.headerItem.height : 0)
                return footerColumn.height + 2 * constant.paddingSmall
            }
            visible: gagModel.busy

            Column {
                id: footerColumn
                anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter }
                height: childrenRect.height

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
        onMovementEnded: mainPage.currentIndex = gagListView.indexAt(gagListView.contentX, gagListView.contentY + 1);

        PullDownMenu {
            MenuItem {
                text: "About GagBook"
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                text: "Settings"
                onClicked: pageStack.push(Qt.resolvedUrl("AppSettingsPage.qml"));
            }            
            MenuItem {
                text: "Refresh"
                onClicked: gagModel.refresh(GagModel.RefreshAll);
            }
        }
    }

    GagModel {
        id: gagModel
        manager: gagbookManager
        onRefreshFailure: infoBanner.alert(errorMessage);
    }

    Connections {
        target: volumeKeyListener
        onVolumeUpClicked: {
            if (currentIndex > 0)
                positionAtIndex(currentIndex - 1, ListView.Beginning);
            else if (currentIndex == 0 && !gagListView.atYBeginning)
                gagListView.positionViewAtBeginning();
        }
        onVolumeDownClicked: {
            if (currentIndex < gagListView.count - 1)
                positionAtIndex(currentIndex + 1, ListView.Beginning);
            else if (currentIndex == gagListView.count - 1 && !gagListView.atYEnd)
                gagListView.positionViewAtEnd();
        }
    }

    Binding {
        target: volumeKeyListener
        property: "enabled"
        value: appSettings.scrollWithVolumeKeys && Qt.application.active && mainPage.status == PageStatus.Active
    }
}
