import QtQuick 1.1
import com.nokia.meego 1.0

Item{
    id: root

    property bool allowDelegateFlicking: gagImage.status === Image.Ready && gagImage.scale > pinchArea.minScale

    height: ListView.view.height
    width: ListView.view.width

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: imageContainer.width
        contentHeight: imageContainer.height
        clip: true
        interactive: allowDelegateFlicking
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
                sourceSize.height: 1750 // Maximum image height
                cache: false
                fillMode: Image.PreserveAspectFit

                function fitToScreen(){
                    scale = Math.min(flickable.width / width, flickable.height / height, 1)
                    pinchArea.minScale = scale
                    prevScale = scale
                }

                onStatusChanged: if(status == Image.Ready) fitToScreen()

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
            onClicked: textContainer.showing = !textContainer.showing
            onDoubleClicked: {
                flickable.returnToBounds()
                if(gagImage.scale !== pinchArea.minScale) fitToScreenAnimation.start()
            }

            NumberAnimation{
                id: fitToScreenAnimation
                target: gagImage
                duration: 250
                property: "scale"
                from: gagImage.scale; to: pinchArea.minScale
            }
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

            Column{
                height: childrenRect.height
                width: busyIndicator.width
                spacing: constant.paddingLarge

                BusyIndicator{
                    id: busyIndicator
                    running: true
                    platformStyle: BusyIndicatorStyle{ size: "large" }
                }

                Label{
                    width: busyIndicator.width
                    horizontalAlignment: Text.AlignHCenter
                    text: Math.round(gagImage.progress/100) + "%"
                }
            }
        }

        Component{
            id: failedLoading

            Label{
                text: "Error loading image!"
            }
        }
    }

    ScrollDecorator{ flickableItem: flickable }

    Rectangle{
        id: textContainer

        property bool showing: true

        width: parent.width
        height: titleText.paintedHeight + 2 * titleText.anchors.margins
        color: "black"
        opacity: 0.65
        y: showing ? 0 : -height
        Behavior on y { NumberAnimation{ duration: 250 } }

        Text{
            id: titleText
            anchors{ left: parent.left; right: parent.right; top: parent.top; margins: constant.paddingMedium }
            font.pixelSize: constant.fontSizeMedium
            font.bold: true
            wrapMode: Text.Wrap
            elide: Text.ElideRight
            maximumLineCount: 3
            color: "white"
            text: model.title + " (" + model.votes + " likes)"
        }
    }
}
