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

Item {
    id: root

    property string text: ""
    property variant buttonsText: []
    property int checkedButtonIndex: 0

    signal buttonClicked(int index)

    width: parent.width
    height: settingText.paintedHeight + buttonRow.height + buttonRow.anchors.topMargin

    Text {
        id: settingText
        anchors { left: parent.left; top: parent.top; leftMargin: constant.paddingMedium }
        font.pixelSize: constant.fontSizeLarge
        color: constant.colorLight
        text: root.text
    }

    ButtonRow {
        id: buttonRow
        anchors {
            top: settingText.bottom
            left: parent.left
            right: parent.right
            margins: constant.paddingSmall
        }
    }

    Component {
        id: buttonComponent

        Button {
            id: thisButton
            platformInverted: settings.whiteTheme
            onClicked: {
                for (var i=0; i<parent.children.length; ++i) {
                    if (parent.children[i] == thisButton) {
                        buttonClicked(i);
                        break;
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        for (var i=0; i<buttonsText.length; ++i) {
            var button = buttonComponent.createObject(buttonRow, { text: buttonsText[i] });
        }
        if (buttonRow.children.length > 0)
            buttonRow.checkedButton = buttonRow.children[checkedButtonIndex]
    }
}
