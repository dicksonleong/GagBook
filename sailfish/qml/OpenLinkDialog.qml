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

Dialog {
    id: openLinkDialog

    property string url

    property bool __buttonClickAccept: false

    onAccepted: {
        // user accept the dialog by swiping to left instead of clicking on the button
        if (!__buttonClickAccept) {
            Qt.openUrlExternally(url);
            infoBanner.alert("Launching web browser...");
        }
    }

    Column {
        anchors { top: parent.top; left: parent.left; right: parent.right }
        spacing: constant.paddingLarge

        DialogHeader { title: "Open URL" }

        Label {
            anchors { left: parent.left; right: parent.right; margins: constant.paddingLarge }
            horizontalAlignment: Text.AlignHCenter
            color: Theme.highlightColor
            wrapMode: Text.WrapAnywhere
            maximumLineCount: 4
            elide: Text.ElideRight
            text: url
        }
    }

    Column {
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom; margins: constant.paddingLarge }

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Open URL in web browser"
            onClicked: {
                Qt.openUrlExternally(url);
                infoBanner.alert("Launching web browser...");
                __buttonClickAccept = true;
                openLinkDialog.accept();
            }
        }

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Copy URL"
            onClicked: {
                QMLUtils.copyToClipboard(url);
                infoBanner.alert("URL copied to clipboard");
                __buttonClickAccept = true;
                openLinkDialog.accept();
            }
        }
    }
}
