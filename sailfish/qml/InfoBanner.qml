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

MouseArea {
    id: infoBanner

    function alert(text) {
        messageText.text = text;
        infoBanner.opacity = 1.0;
        hideTimer.start();
    }

    width: Screen.width
    height: messageText.height + 2 * constant.paddingMedium
    visible: opacity > 0.0
    opacity: 0.0

    Behavior on opacity { FadeAnimation {} }

    Rectangle {
        anchors.fill: parent
        color: Theme.highlightColor
    }

    Text {
        id: messageText
        anchors {
            left: parent.left; right: parent.right; margins: constant.paddingMedium
            verticalCenter: parent.verticalCenter
        }
        font.pixelSize: constant.fontSizeSmall
        color: constant.colorLight
        wrapMode: Text.Wrap
        elide: Text.ElideRight
        maximumLineCount: 3
    }

    Timer {
        id: hideTimer
        interval: 3000
        onTriggered: infoBanner.opacity = 0.0;
    }

    onClicked: {
        opacity = 0.0;
        hideTimer.stop();
    }
}
