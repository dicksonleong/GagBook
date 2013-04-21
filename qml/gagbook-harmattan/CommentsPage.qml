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
import com.nokia.meego 1.0
import QtWebKit 1.0

Page {
    id: commentsPage

    property string gagURL

    tools: ToolBarLayout {
        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: pageStack.pop()
        }
    }

    Flickable {
        id: webViewFlickable
        anchors { top: pageHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        contentHeight: commentsWebView.height * commentsWebView.scale

        WebView {
            id: commentsWebView
            width: commentsBox.width + 20
            scale: webViewFlickable.width / width
            transformOrigin: Item.TopLeft

            javaScriptWindowObjects: QtObject {
                id: commentsBox
                WebView.windowObjectName: "qml"

                property string gagURL: commentsPage.gagURL
                property int width: webViewFlickable.width * 0.75
                property bool whiteTheme: settings.whiteTheme
            }

            onLoadStarted: pageHeader.busy = true
            onLoadFailed: pageHeader.busy = false
            onLoadFinished: pageHeader.busy = false

            Connections { target: appWindow; onInPortraitChanged: commentsWebView.reload.trigger() }
        }
    }

    ScrollDecorator { flickableItem: webViewFlickable }

    PageHeader {
        id: pageHeader
        anchors { top: parent.top; left: parent.left; right: parent.right }
        text: "Comments"
        onClicked: webViewFlickable.contentY = 0;
    }

    Component.onCompleted: commentsWebView.url = "comments.html"
}
