import QtQuick 1.1
import com.nokia.meego 1.0

Item{
    id: root

    height: ListView.view.height
    width: ListView.view.width

    MouseArea{
        anchors.fill: parent
        onClicked: {
            if(textContainer.y == 0) textContainer.y = -textContainer.height
            else textContainer.y = 0
        }
    }

    Flickable{
        id: imageFlickable
        anchors{
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
        }
        height: Math.min(parent.height, gagImage.paintedHeight)
        contentHeight: gagImage.paintedHeight

        Image{
            id: gagImage
            anchors{ left: parent.left; right: parent.right }
            fillMode: Image.PreserveAspectFit
            sourceSize.height: 1500
            cache: false
            source: settings.imageSize === 0 ? model.image.small : model.image.big
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                if(textContainer.y == 0) textContainer.y = -textContainer.height
                else textContainer.y = 0
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

    ScrollDecorator{ flickableItem: imageFlickable }

    Rectangle{
        id: textContainer
        width: parent.width
        height: titleText.paintedHeight + 2 * titleText.anchors.margins
        color: "black"
        opacity: 0.65
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
