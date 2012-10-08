/*
    Copyright (C) 2012  Dickson Leong
    This file is part of GagBook.

    GagBook is free software: you can redistribute it and/or modify it
    under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. If not, see http://www.gnu.org/licenses/.
*/

import QtQuick 1.1
import com.nokia.symbian 1.1

Item{
    id: root

    property bool allowDelegateFlicking: gagImage.status === Image.Ready && gagImage.scale > pinchArea.minScale
    property bool imageZoomed: gagImage.scale !== pinchArea.minScale

    function saveImage(){
        return imageSaver.save(gagImage, model.id)
    }

    function resetImageZoom(){
        flickable.returnToBounds()
        bounceBackAnimation.to = pinchArea.minScale
        bounceBackAnimation.start()
    }

    height: ListView.view.height
    width: ListView.view.width

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: imageContainer.width
        contentHeight: imageContainer.height
        clip: true
        interactive: moving || allowDelegateFlicking
        onHeightChanged: if(gagImage.status == Image.Ready) gagImage.fitToScreen()

        Item {
            id: imageContainer
            width: Math.max(gagImage.width * gagImage.scale, flickable.width)
            height: Math.max(gagImage.height * gagImage.scale, flickable.height)

            Image {
                id: gagImage

                property real prevScale

                anchors.centerIn: parent
                source: settings.imageSize === 0 ? model.image.small : model.image.big
                smooth: !flickable.moving
                sourceSize.height: 2000 // Maximum image height
                cache: false
                fillMode: Image.PreserveAspectFit

                function fitToScreen(){
                    scale = Math.min(flickable.width / width, flickable.height / height, 1)
                    pinchArea.minScale = scale
                    prevScale = scale
                }

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
                    if(status == Image.Ready){
                        fitToScreen()
                        loadedAnimation.start()
                    }
                }

                NumberAnimation{
                    id: loadedAnimation
                    target: gagImage
                    property: "opacity"
                    duration: 250
                    from: 0; to: 1
                    easing.type: Easing.InOutQuad
                }
            }
        }

        PinchArea{
            id: pinchArea

            property real minScale: 1.0
            property real maxScale: 3.0

            anchors.fill: parent
            enabled: gagImage.status === Image.Ready && !root.ListView.view.moving
            pinch.target: gagImage
            pinch.minimumScale: minScale * 0.5 // This is to create "bounce back effect"
            pinch.maximumScale: maxScale * 1.5 // when over zoomed

            onPinchStarted: scrollBar.flickableItem = flickable

            onPinchFinished: {
                flickable.returnToBounds()
                if(gagImage.scale < pinchArea.minScale){
                    bounceBackAnimation.to = pinchArea.minScale
                    bounceBackAnimation.start()
                }
                else if(gagImage.scale > pinchArea.maxScale){
                    bounceBackAnimation.to = pinchArea.maxScale
                    bounceBackAnimation.start()
                }
            }

            NumberAnimation{
                id: bounceBackAnimation
                target: gagImage
                duration: 250
                property: "scale"
                from: gagImage.scale
            }
        }

        MouseArea{
            anchors.fill: parent
            onClicked: textContainer.state = textContainer.state ? "" : "hidden"
        }
    }

    Loader{
        anchors.centerIn: parent
        sourceComponent: {
            switch(gagImage.status){
            case Image.Loading:
                return loadingIndicator
            case Image.Error:
                return failedLoading
            default:
                return undefined
            }
        }

        Component{
            id: loadingIndicator

            BusyIndicator{
                id: busyIndicator
                platformInverted: !settings.whiteTheme
                running: true
                width: platformStyle.graphicSizeLarge; height: width
            }
        }

        Component{ id: failedLoading; Label{ text: "Error loading image" } }
    }

    ScrollDecorator{ id: scrollBar; flickableItem: null; platformInverted: settings.whiteTheme }

    Item{
        id: textContainer
        anchors{ left: parent.left; right: parent.right; top: parent.top }
        height: textColumn.height + 2 * textColumn.anchors.margins
        states: State{
            name: "hidden"
            AnchorChanges{ target: textContainer; anchors.top: undefined; anchors.bottom: root.top }
        }
        transitions: Transition{ AnchorAnimation{ duration: 250; easing.type: Easing.InOutQuad } }

        Rectangle{
            anchors.fill: parent
            color: "black"
            opacity: 0.5
        }

        Column{
            id: textColumn
            anchors{ left: parent.left; right: parent.right; top: parent.top; margins: constant.paddingSmall }
            height: childrenRect.height

            Text{
                width: parent.width
                font.pixelSize: constant.fontSizeMedium
                color: "white"
                font.bold: true
                wrapMode: Text.Wrap
                elide: Text.ElideRight
                maximumLineCount: 2
                text: model.title
            }

            Text{
                width: parent.width
                font.pixelSize: constant.fontSizeMedium
                color: "white"
                elide: Text.ElideRight
                text: model.votes + " likes"
            }
        }
    }
}
