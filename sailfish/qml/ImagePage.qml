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

Page {
    id: imagePage

    property variant gag

    // to make the image outside of the page not visible during page transitions
    clip: true
    transitions: Transition { AnchorAnimation { duration: 250; easing.type: Easing.InOutQuad } }

    property real prevScale: 0
    property real fitScale: 0

    function _onScaleChanged() {
        if (prevScale === 0)
            prevScale = gagImage.scale
        if ((gagImage.width * gagImage.scale) > flick.width) {
            var xoff = (flick.width / 2 + flick.contentX) * gagImage.scale / prevScale;
            flick.contentX = xoff - flick.width / 2
        }
        if ((gagImage.height * gagImage.scale) > flick.height) {
            var yoff = (flick.height / 2 + flick.contentY) * gagImage.scale / prevScale;
            flick.contentY = yoff - flick.height / 2
        }
        prevScale = gagImage.scale
    }

    function _fitToScreen() {
        fitScale = Math.min(flick.width / gagImage.width, flick.height / gagImage.height)
        gagImage.scale = fitScale
        prevScale = fitScale
        pinchArea.pinch.minimumScale = Math.min(fitScale, 1)
    }


    SilicaFlickable {
        id: flick
        anchors.fill: parent
        contentWidth: imageContainer.width
        contentHeight: imageContainer.height
        
        VerticalScrollDecorator{}
        HorizontalScrollDecorator{}

        onHeightChanged: {
           if (gagImage.status === Image.Ready) _fitToScreen();
        }

        Item {
            id: imageContainer
            width: Math.max(gagImage.width * gagImage.scale, flick.width)
            height: Math.max(gagImage.height * gagImage.scale, flick.height)

            Image {
                id: gagImage

                asynchronous: true
                anchors.centerIn: parent
                smooth: !flick.moving
                cache: true
                fillMode: Image.PreserveAspectFit
                sourceSize.height: QMLUtils.IMAGE_MAX_HEIGHT
                source: gag.isPartialImage ? gag.fullImageUrl : gag.imageUrl

                onStatusChanged: {
                    if (status == Image.Ready) {
                        loadedAnimation.start()
                    }
                }

                onScaleChanged: {
                    _onScaleChanged()
                }

                onHeightChanged: {
                    _fitToScreen()
                }

                NumberAnimation {
                    id: loadedAnimation
                    target: gagImage
                    property: "opacity"
                    duration: 250
                    from: 0; to: 1
                    easing.type: Easing.InOutQuad
                }
            } // Image End
        }

        PinchArea {
            id: pinchArea
            anchors.fill: parent

            property real initialWidth
            property real initialHeight
            pinch.target: gagImage
            pinch.minimumScale: 1.0
            pinch.maximumScale: 3.0

            onPinchFinished: {
                // Move its content within bounds.
                flick.returnToBounds()
                flick.interactive = true
            }

            Rectangle {
                opacity: 0.0
                anchors.fill: parent
            }
        }

    }

    Loader {
        anchors.centerIn: parent
        sourceComponent: {
            if (gag.isDownloading)
                return loadingIndicator;

            switch (gagImage.status) {
            case Image.Loading: return loadingIndicator
            case Image.Error: return failedLoading
            default: return undefined
            }
        }

        Component {
            id: loadingIndicator

            BusyIndicator {
                id: busyIndicator
                running: true
                size: BusyIndicatorSize.Large
            }
        }

        Component { id: failedLoading; Label { text: "Error loading image" } }
    }
}

