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
            anchors { left: parent.left; right: parent.right; margins: constant.paddingMedium  }
            font.pixelSize: constant.fontSizeSmall
            color: constant.colorMid
            elide: Text.ElideRight
            text: model.votesCount + " points · " + model.commentsCount + " comments"
        }

        AnimatedImage {
            id: gagImage

            property bool playGif: false

            anchors { left: parent.left; right: parent.right }
            height: (model.imageSize.height * (screen.displayWidth / model.imageSize.width)) || screen.displayWidth
            asynchronous: true
            smooth: !root.ListView.view.moving
            cache: false
            // pause the animation when app is in background
            paused: mainPage.status != PageStatus.Active || !Qt.application.active
            fillMode: Image.PreserveAspectFit
            source: playGif ? model.gifImageUrl : model.imageUrl

            onStatusChanged: if (status == AnimatedImage.Ready) playing = true;

            Loader {
                id: errorTextLoader
                anchors.fill: parent
                sourceComponent: {
                    if (model.isNSFW) return nsfwText;
                    if (!gagImage.source.toString()) {
                        if (model.isDownloading)
                            return downloadingIndicator;
                        return notDownloadedText;
                    }

                    switch (gagImage.status) {
                    case Image.Loading: return loadingRect;
                    case Image.Error: return errorText;
                    case Image.Ready:
                        if (model.isGIF && !gagImage.playGif) return gifPlayIcon;
                        if (model.isVideo) return videoPlayIcon;
                        // fallthrough
                    default: return undefined;
                    }
                }

                Component {
                    id: nsfwText

                    Item {
                        Column {
                            anchors {
                                left: parent.left; right: parent.right
                                verticalCenter: parent.verticalCenter
                            }
                            height: childrenRect.height
                            spacing: constant.paddingMedium

                            Text {
                                anchors { left: parent.left; right: parent.right }
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: constant.fontSizeLarge
                                color: constant.colorLight
                                font.bold: true
                                wrapMode: Text.Wrap
                                text: "Not Safe For Work"
                            }

                            Text {
                                anchors { left: parent.left; right: parent.right }
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: constant.fontSizeMedium
                                color: constant.colorLight
                                wrapMode: Text.Wrap
                                text: "Unfortunately, GagBook does not support viewing NSFW images yet"
                            }
                        }
                    }
                }

                Component {
                    id: notDownloadedText

                    Item {
                        Column {
                            anchors {
                                left: parent.left; right: parent.right
                                verticalCenter: parent.verticalCenter
                            }
                            height: childrenRect.height
                            spacing: constant.paddingMedium

                            Text {
                                anchors { left: parent.left; right: parent.right }
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: constant.fontSizeLarge
                                color: constant.colorLight
                                font.bold: true
                                wrapMode: Text.Wrap
                                text: "Image not downloaded"
                            }

                            Text {
                                anchors { left: parent.left; right: parent.right }
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: constant.fontSizeMedium
                                color: constant.colorLight
                                wrapMode: Text.Wrap
                                text: "Click to download image"
                            }
                        }
                    }
                }

                Component {
                    id: downloadingIndicator

                    Item {
                        BusyIndicator {
                            anchors.centerIn: parent
                            height: platformStyle.graphicSizeLarge
                            width: platformStyle.graphicSizeLarge
                            running: true
                        }
                    }
                }

                Component {
                    id: errorText

                    Text {
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: constant.fontSizeLarge
                        color: constant.colorLight
                        wrapMode: Text.Wrap
                        text: "Error loading image"
                    }
                }

                Component {
                    id: loadingRect

                    Rectangle { color: constant.colorMid }
                }

                Component {
                    id: gifPlayIcon

                    Item {
                        Image {
                            anchors.centerIn: parent
                            source: "Images/icon-gif-play.png"
                        }
                    }
                }

                Component {
                    id: videoPlayIcon

                    Item {
                        Image {
                            anchors.centerIn: parent
                            source: "Images/icon-video-play.png"
                        }
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                enabled: !model.isNSFW && !model.isDownloading
                onClicked: {
                    if (model.isVideo) {
                        Qt.openUrlExternally(model.url);
                    } else if (model.isGIF) {
                        if (!model.gifImageUrl.toString()) {
                            // download GIF
                            gagImage.playGif = true;
                            gagModel.downloadImage(index);
                        } else {
                            gagImage.playGif = !gagImage.playGif;
                            gagImage.playing = true;
                        }
                        return;
                    } else if (!gagImage.source.toString()) {
                        // download image
                        gagModel.downloadImage(index);
                    } else {
                        pageStack.push(Qt.resolvedUrl("ImagePage.qml"), { gag: model });
                    }
                }
            }
        }

        ButtonRow{
            anchors { left: parent.left; right: parent.right; margins: constant.paddingMedium }
            height: childrenRect.height
            exclusive: false

            Button {
                platformInverted: appSettings.whiteTheme
                iconSource: "Images/instant_messenger_chat" + (platformInverted ? "_inverted.svg" : ".svg")
                onClicked: pageStack.push(Qt.resolvedUrl("CommentsPage.qml"), { gagURL: model.url })
            }
            Button {
                platformInverted: appSettings.whiteTheme
                iconSource: "Images/internet" + (platformInverted ? "_inverted.svg" : ".svg")
                onClicked: dialogManager.createOpenLinkDialog(model.url)
            }
            Button {
                platformInverted: appSettings.whiteTheme
                iconSource: "image://theme/toolbar-share" + (platformInverted ? "_inverse" : "")
                onClicked: dialogManager.createShareDialog(model.url)
            }
            Button {
                property string __savedFilePath: ""
                platformInverted: appSettings.whiteTheme
                iconSource: {
                    if (!__savedFilePath)
                        return "Images/download" + (platformInverted ? "_inverted.svg" : ".svg");
                    else
                        return "Images/photos" + (platformInverted ? "_inverted.svg" : ".svg");
                }
                onClicked: {
                    if (!__savedFilePath) {
                        if (model.isGIF && !model.gifImageUrl.toString()) {
                            infoBanner.alert("You have to download the GIF first by clicking on the image");
                            return;
                        }
                        __savedFilePath = QMLUtils.saveImage(model.isGIF ? model.gifImageUrl : model.imageUrl);
                        if (__savedFilePath) {
                            var displayPath = __savedFilePath;
                            if (displayPath.indexOf("file://") == 0)
                                displayPath = displayPath.substring(7);
                            infoBanner.alert("Image saved to " + displayPath);
                        } else {
                            infoBanner.alert("Unable to save image");
                        }
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
