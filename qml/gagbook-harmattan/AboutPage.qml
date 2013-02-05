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

Page {
    id: aboutPage

    tools: ToolBarLayout {
        ToolIcon {
            platformIconId: "toolbar-back"
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
                color: constant.colorMid
                text: "v" + APP_VERSION
            }

            Text {
                anchors { left: parent.left; right: parent.right }
                font.pixelSize: constant.fontSizeMedium
                horizontalAlignment: Text.AlignHCenter
                color: constant.colorLight
                wrapMode: Text.Wrap
                textFormat: Text.PlainText
                text: "GagBook is a 9GAG app that allow you to view the post from 9GAG \
with a simple and native swipe-based UI. \
GagBook is not affiliated with 9GAG.\n\n\
Note: GagBook get the 9GAG posts by parsing HTML from 9GAG website, which can easily failed \
to working anytime if 9GAG modified their HTML, even slightly. \
\n\n\
Copyright © 2012 Dickson\n\
GagBook is open source and licensed under BSD license\n"
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Developer's Website"
                onClicked: Qt.openUrlExternally(constant.devWebSite)
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Source Repository"
                onClicked: Qt.openUrlExternally(constant.sourceRepoSite)
            }
        }
    }

    ScrollDecorator { flickableItem: aboutPageFlickable }

    PageHeader {
        id: pageHeader
        text: "About"
    }
}
