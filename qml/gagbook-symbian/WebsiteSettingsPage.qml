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
    id: websiteSettingsPage

    tools: ToolBarLayout {
        ToolButton {
            platformInverted: gagSettings.whiteTheme
            iconSource: "toolbar-back"
            onClicked: pageStack.pop()
        }
    }

    Flickable {
        id: flickable
        anchors { top: pageHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        contentHeight: column.height + 2 * column.anchors.margins

        Column {
            id: column
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left; right: parent.right
                margins: constant.paddingLarge
            }
            height: childrenRect.height
            spacing: constant.paddingLarge

            SettingSwitch {
                id: cuteSwitch
                text: "Cute"
                checked: websiteSettings.cute
            }

            SettingSwitch {
                id: geekySwitch
                text: "Geeky"
                checked: websiteSettings.geeky
            }

            SettingSwitch {
                id: gifSwitch
                text: "GIF"
                checked: websiteSettings.gif
            }

            SettingSwitch {
                id: showHotInTrendingSwitch
                text: "Show Hot Posts in Trending"
                checked: websiteSettings.showHotInTrending
            }

            SettingSwitch {
                id: showNsfwSwitch
                text: "Show NSFW Posts"
                checked: websiteSettings.showNsfw
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                platformInverted: gagSettings.whiteTheme
                width: parent.width * 0.75
                enabled: !pageHeader.busy
                text: "Save"
                onClicked: {
                    websiteSettings.cute = cuteSwitch.checked;
                    websiteSettings.geeky = geekySwitch.checked;
                    websiteSettings.gif = gifSwitch.checked;
                    websiteSettings.showHotInTrending = showHotInTrendingSwitch.checked;
                    websiteSettings.showNsfw = showNsfwSwitch.checked;
                    websiteSettings.save();
                }
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                platformInverted: gagSettings.whiteTheme
                width: parent.width * 0.75
                enabled: !pageHeader.busy
                text: "Reset"
                onClicked: {
                    cuteSwitch.checked = true;
                    geekySwitch.checked = true;
                    gifSwitch.checked = true;
                    showHotInTrendingSwitch.checked = true;
                    showNsfwSwitch.checked = true;
                    websiteSettings.reset();
                }
            }
        }
    }

    ScrollDecorator { flickableItem: flickable; platformInverted: gagSettings.whiteTheme }

    PageHeader {
        id: pageHeader
        anchors { top: parent.top; left: parent.left; right: parent.right }
        text: "Website Settings"
        enabled: false
        busy: websiteSettings.busy
    }

    WebsiteSettings {
        id: websiteSettings
        manager: gagManager
        onSuccess: infoBanner.alert("Settings saved successfully");
        onFailure: infoBanner.alert(message);
    }
}
