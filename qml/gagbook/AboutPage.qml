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

    Item{
        anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter }
        height: textColumn.height + buttonColumn.anchors.topMargin + buttonColumn.height

        Column{
            id: textColumn
            anchors { left: parent.left; right: parent.right; top: parent.top }
            height: childrenRect.height
            spacing: constant.paddingMedium

            Text{
                width: parent.width
                font.pixelSize: constant.fontSizeXXLarge
                horizontalAlignment: Text.AlignHCenter
                color: constant.colorLight
                text: "GagBook"
            }

            Text{
                width: parent.width
                font.pixelSize: constant.fontSizeMedium
                horizontalAlignment: Text.AlignHCenter
                color: constant.colorMid
                text: "v0.0.2 APLHA"
            }

            Text{
                width: parent.width
                font.pixelSize: constant.fontSizeMedium
                horizontalAlignment: Text.AlignHCenter
                color: constant.colorLight
                wrapMode: Text.Wrap
                text: "GagBook is a (read-only) 9GAG app for your smartphones, allow you to enjoy 9GAGing in your smartphone \
with amazing UI and UX. GagBook is using InfiniGAG API but not affiliated with 9GAG or InfiniGAG in anyway. \
This app may involve transmitting huge amount of data when downloading images.\n\n\
Developed by Dickson.\n\n\
GagBook is licence under GPL v3+."
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

    PageHeader{
        id: pageHeader
        text: "About"
    }
}
