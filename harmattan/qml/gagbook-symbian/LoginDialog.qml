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

CommonDialog {
    id: loginDialog

    property alias username: usernameField.text
    property alias password: passwordField.text

    titleText: "Login"
    buttonTexts: ["Login", "Cancel"]
    onButtonClicked: if (index == 0) accept();

    content: Flickable {
        id: flickable
        anchors { left: parent.left; right: parent.right }
        height: Math.min(platformContentMaximumHeight, column.height + 2 * column.anchors.margins)
        contentHeight: column.height + 2 * column.anchors.margins

        Column {
            id: column
            anchors { left: parent.left; right: parent.right; top: parent.top; margins: constant.paddingMedium }
            spacing: constant.paddingMedium

            Text {
                anchors { left: parent.left; right: parent.right }
                font.pixelSize: constant.fontSizeMedium
                font.bold: true
                color: constant.colorLight
                text: "Email"
            }

            TextField {
                id: usernameField
                anchors { left: parent.left; right: parent.right }
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
                validator: RegExpValidator { regExp: /^\S+$/ }
                Keys.onEnterPressed: passwordField.forceActiveFocus();
            }

            Text {
                anchors { left: parent.left; right: parent.right }
                font.pixelSize: constant.fontSizeMedium
                font.bold: true
                color: constant.colorLight
                text: "Password"
            }

            TextField {
                id: passwordField
                anchors { left: parent.left; right: parent.right }
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
                echoMode: TextInput.Password
                validator: RegExpValidator { regExp: /^\S+$/ }
                Keys.onEnterPressed: loginDialog.accept();
            }

            Text {
                anchors { left: parent.left; right: parent.right }
                font.pixelSize: constant.fontSizeSmall
                color: constant.colorLight
                wrapMode: Text.Wrap
                text: "Login with the email address that you have signed up with 9GAG. " +
                      "Login with Facebook or Google+ is not supported. " +
                      "Your password is not stored by the app."
            }
        }

        ScrollDecorator { flickableItem: flickable }
    }
}
