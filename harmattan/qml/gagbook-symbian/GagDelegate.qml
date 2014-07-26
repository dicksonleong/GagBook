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
import GagBook.Core 1.0

Item {
    id: gagDelegate
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

        Loader {
            id: gagImageLoader

            property bool playGif: false

            anchors { left: parent.left; right: parent.right }
            height: (model.imageSize.height * (screen.displayWidth / model.imageSize.width)) || screen.displayWidth
            sourceComponent: playGif ? animatedImageComponent : imageComponent

            Component {
                id: imageComponent

                Image {
                    asynchronous: true
                    smooth: !gagDelegate.ListView.view.moving
                    cache: false
                    fillMode: Image.PreserveAspectFit
                    source: model.imageUrl
                }
            }

            Component {
                id: animatedImageComponent

                AnimatedImage {
                    asynchronous: true
                    smooth: !gagDelegate.ListView.view.moving
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
                        Column {
                            anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter }
                            spacing: constant.paddingMedium

                            BusyIndicator {
                                anchors.horizontalCenter: parent.horizontalCenter
                                platformInverted: appSettings.whiteTheme
                                height: platformStyle.graphicSizeLarge
                                width: platformStyle.graphicSizeLarge
                                running: true
                            }

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                font.pixelSize: constant.fontSizeMedium
                                color: constant.colorLight
                                text: Math.round(gagModel.manualProgress * 100) + "%"
                            }
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
                    id: partialImageBar

                    Item {
                        Rectangle {
                            anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
                            height: platformStyle.graphicSizeMedium
                            color: "LightSlateGray"

                            Text {
                                anchors.centerIn: parent
                                color: constant.colorLight
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

        ButtonRow{
            anchors { left: parent.left; right: parent.right; margins: constant.paddingMedium }
            height: childrenRect.height
            exclusive: false

            Button {
                platformInverted: appSettings.whiteTheme
                enabled: gagbookManager.loggedIn && !votingManager.busy
                opacity: enabled ? 1.0 : 0.5
                checked: model.likes == 1
                iconSource: "Images/up" + (platformInverted ? "_inverted.svg" : ".svg")
                onClicked: votingManager.vote(model.id, checked ? VotingManager.Unlike : VotingManager.Like);
            }
            Button {
                platformInverted: appSettings.whiteTheme
                enabled: gagbookManager.loggedIn && !votingManager.busy
                opacity: enabled ? 1.0 : 0.5
                checked: model.likes == -1
                iconSource: "Images/down" + (platformInverted ? "_inverted.svg" : ".svg")
                onClicked: votingManager.vote(model.id, checked ? VotingManager.Unlike : VotingManager.Dislike);
            }
            Button {
                platformInverted: appSettings.whiteTheme
                iconSource: "Images/instant_messenger_chat" + (platformInverted ? "_inverted.svg" : ".svg")
                onClicked: pageStack.push(Qt.resolvedUrl("CommentsPage.qml"), { gagURL: model.url })
            }
            Button {
                property Item contextMenu
                platformInverted: appSettings.whiteTheme
                iconSource: "image://theme/toolbar-menu" + (platformInverted ? "_inverse" : "")
                onClicked: {
                    if (!contextMenu)
                        contextMenu = buttonRowContextMenuComponent.createObject(gagDelegate);
                    contextMenu.open();
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

    Component {
        id: buttonRowContextMenuComponent

        ContextMenu {
            platformInverted: appSettings.whiteTheme

            MenuLayout {
                MenuItem {
                    platformInverted: appSettings.whiteTheme
                    text: "Link"
                    onClicked: dialogManager.createOpenLinkDialog(model.url);
                }
                MenuItem {
                    platformInverted: appSettings.whiteTheme
                    text: "Share"
                    onClicked: dialogManager.createShareDialog(model.url);
                }
                MenuItem {
                    platformInverted: appSettings.whiteTheme
                    text: "Save image"
                    onClicked: {
                        if (model.isGIF && !model.gifImageUrl.toString()) {
                            infoBanner.alert("You have to download the GIF first by clicking on the image");
                            return;
                        }
                        var savedFilePath = QMLUtils.saveImage(model.isGIF ? model.gifImageUrl : model.imageUrl);
                        if (savedFilePath) {
                            if (savedFilePath.indexOf("file://") == 0)
                                savedFilePath = savedFilePath.substring(7);
                            infoBanner.alert("Image saved to " + savedFilePath);
                        } else {
                            infoBanner.alert("Unable to save image");
                        }
                    }
                }
            }
        }
    }
}
