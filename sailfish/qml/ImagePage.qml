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

    SilicaFlickable {
        id: flick
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: parent.height
        
        VerticalScrollDecorator{}
        HorizontalScrollDecorator{}

        PinchArea {
            width: Math.max(flick.contentWidth, flick.width)
            height: Math.max(flick.contentHeight, flick.height)

            property real initialWidth
            property real initialHeight
            onPinchStarted: {
                initialWidth = flick.contentWidth
                initialHeight = flick.contentHeight
                flick.interactive = false
            }

            onPinchUpdated: {
                // adjust content pos due to drag
                flick.contentX += pinch.previousCenter.x - pinch.center.x
                flick.contentY += pinch.previousCenter.y - pinch.center.y

                // resize content
                flick.resizeContent(initialWidth * pinch.scale, initialHeight * pinch.scale, pinch.center)
            }

            onPinchFinished: {
                // Move its content within bounds.
                flick.returnToBounds()
                flick.interactive = true
            }
            Image {
                id: gagImage

                property real prevScale
                
                width: flick.contentWidth
                height: flick.contentHeight

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

