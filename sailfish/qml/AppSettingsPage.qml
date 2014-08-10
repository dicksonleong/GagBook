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
    id: settingsPage

    readonly property bool busy: gagbookManager.busy

    SilicaFlickable {
        id: settingsFlickable
        anchors.fill: parent
        contentHeight: settingsColumn.height

        Column {
            id: settingsColumn
            anchors { left: parent.left; right: parent.right }
            height: childrenRect.height

            PageHeader { title: "App Settings" }

            ComboBox {
                anchors { left: parent.left; right: parent.right }
                label: "Source"
                currentIndex: appSettings.source
                menu: ContextMenu {
                    MenuItem { text: "9GAG" }
                    MenuItem { text: "InfiniGAG" }
                }
                onCurrentIndexChanged: {
                    switch (currentIndex) {
                    case 0: appSettings.source = AppSettings.NineGagSource; break;
                    case 1: appSettings.source = AppSettings.InfiniGagSource; break;
                    }
                }
            }

            TextSwitch {
                anchors { left: parent.left; right: parent.right }
                text: "Scroll with volume keys"
                checked: appSettings.scrollWithVolumeKeys
                onCheckedChanged: appSettings.scrollWithVolumeKeys = checked;
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: !gagbookManager.busy
                text: appSettings.loggedIn ? "Log out" : "Login to 9GAG"
                onClicked: {
                    if (appSettings.loggedIn) {
                        gagbookManager.logout();
                        infoBanner.alert("Logged out from 9GAG");
                    } else {
                        var dialog = pageStack.push(Qt.resolvedUrl("LoginDialog.qml"));
                        dialog.accepted.connect(function() {
                            gagbookManager.login(dialog.username, dialog.password);
                        })
                    }
                }
            }
        }

        VerticalScrollDecorator {}
    }
}
