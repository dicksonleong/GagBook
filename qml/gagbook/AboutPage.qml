import QtQuick 1.1
import com.nokia.meego 1.0

Page{
    id: aboutPage

    tools: ToolBarLayout{
        ToolIcon{
            platformIconId: "toolbar-back"
            onClicked: pageStack.pop()
        }
    }

    Flickable{
        id: aboutPageFlickable
        anchors{ top: pageHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        contentHeight: container.height

        Item{
            id: container
            width: aboutPageFlickable.width
            height: textColumn.height + buttonColumn.anchors.topMargin + buttonColumn.height + 2 * textColumn.anchors.margins

            Column{
                id: textColumn
                anchors { left: parent.left; right: parent.right; top: parent.top; margins: constant.paddingMedium }
                height: childrenRect.height
                spacing: constant.paddingMedium

                Text{
                    width: parent.width
                    font.pixelSize: constant.fontSizeXXLarge
                    horizontalAlignment: Text.AlignHCenter
                    color: constant.colorLight
                    font.bold: true
                    text: "GagBook"
                }

                Text{
                    width: parent.width
                    font.pixelSize: constant.fontSizeMedium
                    horizontalAlignment: Text.AlignHCenter
                    color: constant.colorMid
                    text: "v0.1 BETA"
                }

                Text{
                    width: parent.width
                    font.pixelSize: constant.fontSizeMedium
                    horizontalAlignment: Text.AlignHCenter
                    color: constant.colorLight
                    wrapMode: Text.Wrap
                    textFormat: Text.PlainText
                    text: "GagBook is a (read-only) 9GAG app for your smartphones, allow you to enjoy 9GAGing in your \
smartphone with a simple swipe UI. GagBook is using InfiniGAG API but not affiliated with 9GAG or InfiniGAG. \
This app may involve transmitting huge amount of data when downloading images.\n\n\
Copyright © 2012 Dickson\n\
GagBook is open source and licensed under GPL v3"
                }
            }

            Column{
                id: buttonColumn
                anchors{ top: textColumn.bottom; topMargin: constant.paddingXXLarge; horizontalCenter: parent.horizontalCenter }
                width: 322; height: childrenRect.height
                spacing: constant.paddingMedium

                Button{
                    text: "Developer's Website"
                    onClicked: Qt.openUrlExternally(constant.devWebSite)
                }

                Button{
                    text: "Source Repository"
                    onClicked: Qt.openUrlExternally(constant.sourceRepoSite)
                }
            }
        }
    }

    ScrollDecorator { flickableItem: aboutPageFlickable }

    PageHeader{
        id: pageHeader
        text: "About"
    }
}
