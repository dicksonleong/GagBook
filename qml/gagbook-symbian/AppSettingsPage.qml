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
import GagBook 1.0

Page {
    id: settingsPage

    tools: ToolBarLayout {
        ToolButton {
            platformInverted: appSettings.whiteTheme
            iconSource: "toolbar-back"
            onClicked: pageStack.pop()
        }
    }

    Flickable {
        id: settingsFlickable
        anchors { top: pageHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        contentHeight: settingsColumn.height + 2 * settingsColumn.anchors.margins

        Column {
            id: settingsColumn
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left; right: parent.right
                margins: constant.paddingMedium
            }
            height: childrenRect.height
            spacing: constant.paddingLarge

            SettingButtonRow {
                text: "Theme"
                checkedButtonIndex: appSettings.whiteTheme ? 1 : 0
                buttonsText: ["Dark", "White"]
                onButtonClicked: appSettings.whiteTheme = index === 1
            }

            SettingButtonRow {
                text: "Source"
                checkedButtonIndex: {
                    switch (appSettings.source) {
                    case AppSettings.NineGagSource: return 0;
                    case AppSettings.InfiniGagSource: return 1;
                    }
                }
                buttonsText: ["9GAG", "InfiniGAG"]
                onButtonClicked: {
                    switch (index) {
                    case 0: appSettings.source = AppSettings.NineGagSource; break;
                    case 1: appSettings.source = AppSettings.InfiniGagSource; break;
                    }
                }
            }

            SettingButtonRow {
                text: "Download GIFs automatically"
                buttonsText: ["On", "Wi-Fi only", "Off"]
                checkedButtonIndex: {
                    switch (appSettings.gifDownloadMode) {
                    case AppSettings.GifDownloadOn: return 0;
                    case AppSettings.GifDownloadOnWiFiOnly: return 1;
                    case AppSettings.GifDownloadOff: return 2;
                    }
                }
                onButtonClicked: {
                    switch (index) {
                    case 0: appSettings.gifDownloadMode = AppSettings.GifDownloadOn; break;
                    case 1: appSettings.gifDownloadMode = AppSettings.GifDownloadOnWiFiOnly; break;
                    case 2: appSettings.gifDownloadMode = AppSettings.GifDownloadOff; break;
                    }
                }
            }
        }
    }

    ScrollDecorator { platformInverted: appSettings.whiteTheme; flickableItem: settingsFlickable }

    PageHeader {
        id: pageHeader
        anchors { top: parent.top; left: parent.left; right: parent.right }
        text: "App Settings"
        enabled: false
    }
}
