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
    width: ListView.view.width
    height: mainColumn.height + 2 * mainColumn.anchors.topMargin + seperator.height

    Column {
        id: mainColumn
        anchors { left: parent.left; right: parent.right; top: parent.top; topMargin: constant.paddingLarge }
        height: childrenRect.height
        spacing: constant.paddingMedium

        Text {
            anchors { left: parent.left; right: parent.right; margins: constant.paddingMedium }
            font.pixelSize: constant.fontSizeMedium
            font.bold: true
            color: constant.colorLight
            wrapMode: Text.Wrap
            text: model.title
        }

        Text {
            anchors { left: parent.left; right: parent.right; margins: constant.paddingMedium }
            font.pixelSize: constant.fontSizeSmall
            color: constant.colorMid
            elide: Text.ElideRight
            text: {
                var t = (model.votesCount == 1 ? "1 like" : model.votesCount + " likes");
                if (model.commentsCount > 0)
                    t += " · " + (model.commentsCount == 1 ? "1 comment" : model.commentsCount + " comments");
                if (model.isVideo)
                    t += " · Video";
                if (model.isNSFW)
                    t += " · NSFW";
                return t;
            }
        }

        Image {
            id: gagImage

            function __calculateImageHeight() {
                var width = 460, height = model.imageHeight;
                if (height > QMLUtils.IMAGE_MAX_HEIGHT) {
                    width *= QMLUtils.IMAGE_MAX_HEIGHT / height;
                    height = QMLUtils.IMAGE_MAX_HEIGHT;
                }
                if (width > gagImage.width)
                   height *= gagImage.width / width;
                return height || width;
            }

            anchors { left: parent.left; right: parent.right }
            height: __calculateImageHeight()
            sourceSize.height: QMLUtils.IMAGE_MAX_HEIGHT
            asynchronous: true
            smooth: !root.ListView.view.moving
            cache: false
            fillMode: Image.PreserveAspectFit
            source: model.imageUrl

            Loader {
                id: errorTextLoader
                anchors.fill: parent
                sourceComponent: {
                    switch (gagImage.status) {
                    case Image.Loading: return loadingRect;
                    case Image.Error: return errorText;
                    default: return undefined;
                    }
                }

                Component {
                    id: errorText

                    Text {
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: constant.fontSizeLarge
                        color: constant.colorLight
                        text: "Error loading image"
                    }
                }

                Component {
                    id: loadingRect

                    Rectangle { color: constant.colorMid }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: pageStack.push(Qt.resolvedUrl("ImagePage.qml"), { imageUrl: model.imageUrl })
            }
        }

        ButtonRow{
            anchors { left: parent.left; right: parent.right; margins: constant.paddingMedium }
            height: childrenRect.height
            exclusive: false

            Button {
                platformInverted: settings.whiteTheme
                iconSource: "Images/instant_messenger_chat" + (platformInverted ? "_inverted.svg" : ".svg")
                onClicked: pageStack.push(Qt.resolvedUrl("CommentsPage.qml"), { gagURL: model.url })
            }
            Button {
                platformInverted: settings.whiteTheme
                iconSource: "Images/internet" + (platformInverted ? "_inverted.svg" : ".svg")
                onClicked: dialogManager.createOpenLinkDialog(model.url)
            }
            Button {
                platformInverted: settings.whiteTheme
                iconSource: "image://theme/toolbar-share" + (platformInverted ? "_inverse" : "")
                onClicked: dialogManager.createShareDialog(model.url)
            }
            Button {
                property string __savedFilePath: ""
                platformInverted: settings.whiteTheme
                iconSource: {
                    if (!__savedFilePath)
                        return "Images/download" + (platformInverted ? "_inverted.svg" : ".svg");
                    else
                        return "Images/photos" + (platformInverted ? "_inverted.svg" : ".svg");
                }
                onClicked: {
                    if (!__savedFilePath) {
                        __savedFilePath = QMLUtils.saveImage(model.imageUrl);
                        if (__savedFilePath)
                            infoBanner.alert("Image saved to " + __savedFilePath);
                        else
                            infoBanner.alert("Unable to save image");
                    } else {
                        Qt.openUrlExternally(__savedFilePath);
                        __savedFilePath = "";
                    }
                }
            }
        }
    }

    Rectangle {
        id: seperator
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
        height: 1
        color: constant.colorMid
    }
}
