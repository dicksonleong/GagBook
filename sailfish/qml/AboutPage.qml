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

Page {
    id: aboutPage

    Flickable {
        id: aboutPageFlickable
        anchors { top: pageHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        contentHeight: column.height + 2 * column.anchors.margins

        Column {
            id: column
            anchors { left: parent.left; right: parent.right; top: parent.top; margins: Theme.paddingMedium }
            height: childrenRect.height
            spacing: Theme.paddingMedium

            Text {
                anchors { left: parent.left; right: parent.right }
                font.pixelSize: Theme.fontSizeExtraLarge
                horizontalAlignment: Text.AlignHCenter
                color: Theme.primaryColor
                font.bold: true
                text: "GagBook"
            }

            Text {
                anchors { left: parent.left; right: parent.right }
                font.pixelSize: Theme.fontSizeMedium
                horizontalAlignment: Text.AlignHCenter
                color: Theme.primaryColor
                wrapMode: Text.Wrap
                textFormat: Text.RichText
                text: "v" + APP_VERSION + "<br>\
Copyright (c) 2014 Dickson Leong<br><br>\
Sailfish port by <a href=\"http://twitter.com/b0bben\">b0bben</a><br><br>\
GagBook is an unofficial mobile app for 9GAG website that allows you to view posts from \
9GAG easily with a simple, smooth and native user interface.<br><br>\
By using this app you agree to 9GAG's <a href=\"http://9gag.com/tos\">Terms of Service</a> \
and <a href=\"http://9gag.com/privacy\">Privacy Policy</a>.<br>"
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Text {
                anchors { left: parent.left; right: parent.right }
                font.pixelSize: Theme.fontSizeMedium
                horizontalAlignment: Text.AlignHCenter
                color: Theme.primaryColor
                wrapMode: Text.Wrap
                textFormat: Text.PlainText
                text: "~ " + gagbookManager.downloadCounter + " MB downloaded"
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Developer's Website"
                onClicked: Qt.openUrlExternally(QMLUtils.DEV_WEBSITE)
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Source Repository"
                onClicked: Qt.openUrlExternally(QMLUtils.REPO_WEBSITE)
            }
        }
    }

    ScrollDecorator { flickable: aboutPageFlickable }

    PageHeader {
        id: pageHeader
        title: "About"
    }
}
