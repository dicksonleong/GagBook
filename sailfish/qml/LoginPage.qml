/*
 * Copyright (c) 2014 Bob Jelica (@b0bben)
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
    property string username
    property string password

    Column {
        spacing: 10
        anchors.fill: parent

        DialogHeader {
            acceptText: "Login"
        }

        TextField {
            id: usernameField
            width: 480
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            placeholderText: "9gag.com username"
        }

        TextField {
            id: passwordField
            width: 480
            echoMode: TextInput.Password
            placeholderText: "Password"
        }
        Text {
            id: wrongLoginDetails
            text: "Wrong username or password"
            color: "red"
            visible: false
        }
    }

    onDone: {
        if (result == DialogResult.Accepted) {
            console.log("accepted dialog");
            username = usernameField.text;
            password = passwordField.text;

            passwordField.text = "Password";

            gagbookManager.login(username, password);
            acceptPending = true;
        }
    }

    Connections {
        target: gagbookManager
        onLoggedInChanged: {
            if (!gagbookManager.loggedIn)
                wrongLoginDetails.visible = true;
            else {
                wrongLoginDetails.visible = false;
                accept();
            }
        }
    }
}

