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
import harbour.gagbook.Core 1.0

Item {
    id: root
    width: ListView.view.width
    height: mainColumn.height + separator.height

    Column {
        id: mainColumn
        anchors { left: parent.left; right: parent.right }
        height: childrenRect.height
        spacing: Theme.paddingMedium

        Text {
            anchors { left: parent.left; right: parent.right; margins: Theme.paddingMedium }
            font.pixelSize: Theme.fontSizeMedium
            font.bold: true
            color: Theme.primaryColor
            wrapMode: Text.Wrap
            text: model.title
        }

        Text {
            anchors { left: parent.left; right: parent.right; margins: Theme.paddingMedium }
            font.pixelSize: Theme.fontSizeSmall
            color: Theme.secondaryColor
            text: model.votesCount + " points · " + model.commentsCount + " comments"
        }

        Loader {
            id: gagImageLoader

            property bool playGif: false

            anchors { left: parent.left; right: parent.right }
            height: (model.imageSize.height * (Screen.width / model.imageSize.width)) || Screen.width
            sourceComponent: playGif ? animatedImageComponent : imageComponent

            Component {
                id: imageComponent

                Image {
                    asynchronous: true
                    smooth: !root.ListView.view.moving
                    cache: false
                    fillMode: Image.PreserveAspectFit
                    source: model.imageUrl
                }
            }

            Component {
                id: animatedImageComponent

                AnimatedImage {
                    asynchronous: true
                    smooth: !root.ListView.view.moving
                    cache: false
                    // pause the animation when app is in background
                    paused: mainPage.status != PageStatus.Active || !Qt.application.active
                    fillMode: Image.PreserveAspectFit
                    source: model.gifImageUrl
                }
            }

            Loader {
                anchors.fill: parent
                z: 1
                sourceComponent: {
                    switch (gagImageLoader.item.status) {
                    case Image.Null:
                        if (model.isNSFW)
                            return nsfwText;
                        if (model.isDownloading)
                            return downloadingIndicator;
                        return notDownloadedText;
                    case Image.Loading:
                        return loadingRect;
                    case Image.Error:
                        return errorText;
                    case Image.Ready:
                        if (model.isGIF && !gagImageLoader.playGif)
                            return gifPlayIcon;
                        if (model.isPartialImage)
                            return partialImageBar;
                        return undefined;
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
                            spacing: Theme.paddingMedium

                            Text {
                                anchors { left: parent.left; right: parent.right; margins: constant.paddingMedium }
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: Theme.fontSizeLarge
                                color: Theme.primaryColor
                                font.bold: true
                                wrapMode: Text.Wrap
                                text: "Not Safe For Work"
                            }

                            Text {
                                anchors { left: parent.left; right: parent.right; margins: constant.paddingMedium }
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                wrapMode: Text.Wrap
                                text: gagbookManager.loggedIn ? "You need to enable showing of NSFW posts " +
                                                                "for your account at 9GAG website."
                                                              : "You need to login to view NSFW images."
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
                            spacing: Theme.paddingMedium

                            Text {
                                anchors { left: parent.left; right: parent.right; margins: constant.paddingMedium }
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: Theme.fontSizeLarge
                                color: Theme.primaryColor
                                font.bold: true
                                wrapMode: Text.Wrap
                                text: "Image not downloaded"
                            }

                            Text {
                                anchors { left: parent.left; right: parent.right; margins: constant.paddingMedium }
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
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
                            running: true
                            size: BusyIndicatorSize.Large
                        }

                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: constant.fontSizeSmall
                            color: constant.colorLight
                            text: Math.round(gagModel.manualProgress * 100) + "%"
                        }
                    }
                }

                Component {
                    id: errorText

                    Text {
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: Theme.fontSizeLarge
                        color: Theme.primaryColor
                        wrapMode: Text.Wrap
                        text: "Error loading image"
                    }
                }

                Component {
                    id: loadingRect

                    Rectangle { color: "#000" }
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
                    id: partialImageBar

                    Item {
                        Rectangle {
                            anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
                            height: Theme.itemSizeSmall
                            color: "LightSlateGray"

                            Label {
                                anchors.centerIn: parent
                                font.pixelSize: constant.fontSizeSmall
                                text: "Click to see full image"
                            }
                        }
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                enabled: !model.isNSFW && !model.isDownloading
                onClicked: {
                    if (model.isGIF) {
                        if (!model.gifImageUrl.toString()) {
                            // download GIF
                            gagImageLoader.playGif = true;
                            gagModel.downloadImage(index);
                        } else {
                            gagImageLoader.playGif = !gagImageLoader.playGif;
                        }
                        return;
                    } else if (gagImageLoader.item.status == Image.Null) {
                        // download image
                        gagModel.downloadImage(index);
                    } else {
                        pageStack.push(Qt.resolvedUrl("ImagePage.qml"), { gag: model });
                    }
                }
            }
        }

        Row {
            anchors { left:parent.left; right:parent.right; margins: Theme.paddingMedium }
            height: childrenRect.height
            spacing: constant.paddingMedium

            IconButton {
                enabled: gagbookManager.loggedIn && !votingManager.busy
                icon.source: "image://theme/icon-m-up"
                highlighted: model.likes == 1
                onClicked: votingManager.vote(model.id, highlighted ? VotingManager.Unlike : VotingManager.Like);
            }
            IconButton {
                enabled: gagbookManager.loggedIn && !votingManager.busy
                icon.source: "image://theme/icon-m-down"
                highlighted: model.likes == -1
                onClicked: votingManager.vote(model.id, highlighted ? VotingManager.Unlike : VotingManager.Dislike);
            }
            IconButton {
                icon.height: Theme.iconSizeMedium; icon.width: Theme.iconSizeMedium
                icon.source: "image://theme/icon-m-message"
                onClicked: pageStack.push(Qt.resolvedUrl("CommentsPage.qml"), { gagURL: model.url })
            }
            IconButton {
                icon.height: Theme.iconSizeMedium; icon.width: Theme.iconSizeMedium
                icon.source: "image://theme/icon-lock-social"
                onClicked: pageStack.push(Qt.resolvedUrl("OpenLinkDialog.qml"), { url: model.url });
            }
            /* Sailfish does not has (public) sharing API yet
            IconButton {
                icon.height: Theme.iconSizeMedium; icon.width: Theme.iconSizeMedium
                icon.source: "image://theme/icon-m-share"
                onClicked: QMLUtils.shareLink(model.url, model.title)
            }*/
            IconButton {
                property string __savedFilePath: ""
                icon.height: Theme.iconSizeMedium; icon.width: Theme.iconSizeMedium
                icon.source: "image://theme/icon-m-" + (__savedFilePath ? "image" : "download")
                onClicked: {
                    if (!__savedFilePath) {
                        if (model.isGIF && !model.gifImageUrl.toString()) {
                            infoBanner.alert("You have to download the GIF first by clicking on the image");
                            return;
                        }
                        __savedFilePath = QMLUtils.saveImage(model.isGIF ? model.gifImageUrl : model.imageUrl);
                        if (__savedFilePath) {
                            var displayPath = __savedFilePath;
                            if (__savedFilePath.indexOf("file://") == 0)
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

    Separator {
        id: separator
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
        color: Theme.secondaryColor
    }
}
