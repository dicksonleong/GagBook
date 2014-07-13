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

CoverBackground {
    id: cover

    property Item mainPage: pageStack.find(function(t) { return t.objectName == "mainPage"; } )

    property bool __increamentIndexAfterLoad: false

    SlideshowView {
        id: slideView
        anchors.fill: parent
        visible: !mainPage.gagModel.busy
        model: mainPage.gagModel
        delegate: Image {
            id: gagImage
            height: slideView.height
            width: slideView.width
            clip: true
            fillMode: Image.PreserveAspectFit
            cache: false
            source: model.imageUrl

            Loader {
                anchors.centerIn: parent
                sourceComponent: {
                    switch (gagImage.status) {
                    case Image.Null:
                        if (model.isNSFW)
                            return nsfwText;
                        return notDownloadedText;
                    case Image.Error:
                        return errorText;
                    case Image.Ready:
                        if (model.isGIF)
                            return gifPlayIcon;
                        if (model.isVideo)
                            return videoPlayIcon;
                        return undefined;
                    }
                }

                Component {
                    id: nsfwText
                    Label { width: gagImage.width; wrapMode: Text.Wrap; text: "NSFW" }
                }

                Component {
                    id: notDownloadedText
                    Label { width: gagImage.width; wrapMode: Text.Wrap; text: "Image not downloaded" }
                }

                Component {
                    id: errorText
                    Label { width: gagImage.width; wrapMode: Text.Wrap; text: "Error loading image" }
                }

                Component {
                    id: gifPlayIcon
                    Image { source: "Images/icon-gif-play.png" }
                }

                Component {
                    id: videoPlayIcon
                    Image { source: "Images/icon-video-play.png" }
                }
            }
        }

        onCountChanged: {
            if (__increamentIndexAfterLoad) {
                __increamentIndexAfterLoad = false;
                incrementCurrentIndex();
            }
        }

        Binding {
            target: slideView
            property: "currentIndex"
            value: mainPage.currentIndex
            when: cover.status == Cover.Inactive
        }
    }

    Label {
        anchors.centerIn: parent
        opacity: mainPage.gagModel.busy ? 1 : 0
        visible: opacity != 0
        font.pixelSize: constant.fontSizeLarge
        color: constant.colorLight
        text: "Loading"

        Behavior on opacity { PropertyAnimation { duration: 250 } }
    }

    CoverActionList {
        enabled: !mainPage.gagModel.busy
        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: mainPage.gagModel.refresh(GagModel.RefreshAll);
        }
        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: {
                if (slideView.currentIndex == slideView.count - 1) {
                    __increamentIndexAfterLoad = true;
                    mainPage.gagModel.refresh(GagModel.RefreshOlder);
                } else {
                    slideView.incrementCurrentIndex();
                }
            }
        }
    }

    onStatusChanged: {
        // when cover is deactivating means app is activating
        if (status == Cover.Deactivating)
            mainPage.positionAtIndex(slideView.currentIndex);
    }
}
