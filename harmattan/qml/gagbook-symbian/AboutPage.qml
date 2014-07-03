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

Page {
    id: aboutPage

    tools: ToolBarLayout {
        ToolButton {
            platformInverted: appSettings.whiteTheme
            iconSource: "toolbar-back"
            onClicked: pageStack.pop()
        }
    }

    Flickable {
        id: aboutPageFlickable
        anchors { top: pageHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        contentHeight: column.height + 2 * column.anchors.margins

        Column {
            id: column
            anchors { left: parent.left; right: parent.right; top: parent.top; margins: constant.paddingMedium }
            height: childrenRect.height
            spacing: constant.paddingMedium

            Text {
                anchors { left: parent.left; right: parent.right }
                font.pixelSize: constant.fontSizeXXLarge
                horizontalAlignment: Text.AlignHCenter
                color: constant.colorLight
                font.bold: true
                text: "GagBook"
            }

            Text {
                anchors { left: parent.left; right: parent.right }
                font.pixelSize: constant.fontSizeMedium
                horizontalAlignment: Text.AlignHCenter
                color: constant.colorLight
                wrapMode: Text.Wrap
                textFormat: Text.RichText
                text: "v" + APP_VERSION + "<br>\
Copyright (c) 2014 Dickson Leong<br><br>\
GagBook is an unofficial mobile app for 9GAG website that allows you to view posts from \
9GAG easily with a simple, smooth and native user interface.<br><br>\
By using this app you agree to 9GAG's <a href=\"http://9gag.com/tos\">Terms of Service</a> \
and <a href=\"http://9gag.com/privacy\">Privacy Policy</a>.<br>"
                onLinkActivated: QMLUtils.openDefaultBrowser(link)
            }

            Text {
                anchors { left: parent.left; right: parent.right }
                font.pixelSize: constant.fontSizeMedium
                horizontalAlignment: Text.AlignHCenter
                color: constant.colorLight
                wrapMode: Text.Wrap
                textFormat: Text.PlainText
                text: "~ " + gagbookManager.downloadCounter + " MB downloaded"
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                platformInverted: appSettings.whiteTheme
                text: "Source Repository"
                width: parent.width * 0.75
                onClicked: QMLUtils.openDefaultBrowser(QMLUtils.REPO_WEBSITE)
            }
        }
    }

    ScrollDecorator { flickableItem: aboutPageFlickable; platformInverted: appSettings.whiteTheme }

    PageHeader {
        id: pageHeader
        anchors { top: parent.top; left: parent.left; right: parent.right }
        text: "About"
        enabled: false
    }
}
