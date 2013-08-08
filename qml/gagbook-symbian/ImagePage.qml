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

Page {
    id: imagePage

    property alias imageUrl: gagImage.source

    tools: ToolBarLayout {
        ToolButton {
            platformInverted: settings.whiteTheme
            iconSource: "toolbar-back"
            onClicked: pageStack.pop()
        }
        ToolButton {
            platformInverted: settings.whiteTheme
            iconSource: "Images/undo" + (platformInverted ? "_inverted.svg" : ".svg")
            opacity: enabled ? 1 : 0.25
            enabled: gagImage.scale != pinchArea.minScale
            onClicked: {
                flickable.returnToBounds()
                bounceBackAnimation.to = pinchArea.minScale
                bounceBackAnimation.start()
            }
        }
    }

    // to make the image outside of the page not visible during page transitions
    clip: true

    states: State {
        name: "hidden"
        AnchorChanges { target: zoomSlider; anchors.right: undefined; anchors.left: root.right }
    }
    transitions: Transition { AnchorAnimation { duration: 250; easing.type: Easing.InOutQuad } }

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: imageContainer.width; contentHeight: imageContainer.height
        onHeightChanged: if (gagImage.status == Image.Ready) gagImage.fitToScreen()

        Item {
            id: imageContainer
            width: Math.max(gagImage.width * gagImage.scale, flickable.width)
            height: Math.max(gagImage.height * gagImage.scale, flickable.height)

            AnimatedImage {
                id: gagImage

                property real prevScale

                function fitToScreen() {
                    scale = Math.min(flickable.width / width, flickable.height / height, 1)
                    pinchArea.minScale = scale
                    prevScale = scale
                }

                asynchronous: true
                anchors.centerIn: parent
                smooth: !flickable.moving
                cache: false
                fillMode: Image.PreserveAspectFit
                // pause the animation when app is in background
                paused: imagePage.status != PageStatus.Active || !symbian.foreground

                onScaleChanged: {
                    if ((width * scale) > flickable.width) {
                        var xoff = (flickable.width / 2 + flickable.contentX) * scale / prevScale;
                        flickable.contentX = xoff - flickable.width / 2
                    }
                    if ((height * scale) > flickable.height) {
                        var yoff = (flickable.height / 2 + flickable.contentY) * scale / prevScale;
                        flickable.contentY = yoff - flickable.height / 2
                    }
                    prevScale = scale
                }

                onStatusChanged: {
                    if (status == Image.Ready) {
                        fitToScreen()
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
            }
        }

        PinchArea {
            id: pinchArea

            property real minScale: 1.0
            property real maxScale: 3.0

            anchors.fill: parent
            enabled: gagImage.status == Image.Ready
            pinch.target: gagImage
            pinch.minimumScale: minScale * 0.5 // This is to create "bounce back effect"
            pinch.maximumScale: maxScale * 1.5 // when over zoomed

            onPinchFinished: {
                flickable.returnToBounds()
                if (gagImage.scale < pinchArea.minScale) {
                    bounceBackAnimation.to = pinchArea.minScale
                    bounceBackAnimation.start()
                }
                else if (gagImage.scale > pinchArea.maxScale) {
                    bounceBackAnimation.to = pinchArea.maxScale
                    bounceBackAnimation.start()
                }
            }

            NumberAnimation {
                id: bounceBackAnimation
                target: gagImage
                duration: 250
                property: "scale"
                from: gagImage.scale
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: imagePage.state = (imagePage.state ? "" : "hidden")
        }
    }

    Loader {
        anchors.centerIn: parent
        sourceComponent: {
            switch (gagImage.status) {
            case Image.Loading:
                return loadingIndicator
            case Image.Error:
                return failedLoading
            default:
                return undefined
            }
        }

        Component {
            id: loadingIndicator

            BusyIndicator {
                id: busyIndicator
                platformInverted: !settings.whiteTheme
                running: true
                width: platformStyle.graphicSizeLarge; height: width
            }
        }

        Component { id: failedLoading; Label { text: "Error loading image"; platformInverted: settings.whiteTheme } }
    }

    ScrollDecorator { id: scrollBar; flickableItem: flickable; platformInverted: settings.whiteTheme }

    Slider {
        id: zoomSlider
        anchors { verticalCenter: parent.verticalCenter; right: parent.right; rightMargin: constant.paddingMedium }
        platformInverted: settings.whiteTheme
        enabled: gagImage.status == Image.Ready
        height: parent.height * 0.6
        opacity: pressed ? 1 : 0.6
        inverted: true
        minimumValue: pinchArea.minScale
        maximumValue: pinchArea.maxScale
        stepSize: (maximumValue - minimumValue) / 20
        orientation: Qt.Vertical

        Behavior on opacity { NumberAnimation { duration: 150 } }

        // When not pressed, bind slider value to image scale
        Binding {
            target: zoomSlider
            property: "value"
            value: gagImage.scale
            when: !zoomSlider.presseds
        }

        // When pressed, bind image scale to slider value
        Binding {
            target: gagImage
            property: "scale"
            value: zoomSlider.value
            when: zoomSlider.pressed
        }
    }
}
