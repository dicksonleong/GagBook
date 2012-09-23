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

    Column{
        id: textColumn
        anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter }
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
            text: "v0.0.1 APLHA"
        }

        Text{
            width: parent.width
            font.pixelSize: constant.fontSizeMedium
            horizontalAlignment: Text.AlignHCenter
            color: constant.colorLight
            wrapMode: Text.Wrap
            text: "GagBook is a (read-only) 9GAG app for your smartphones, allow you to enjoy 9GAGing in your smartphone \
with amazing UI and UX. GagBook is using InfiniGAG API but not affiliated with 9GAG or InfiniGAG in anyway. \
Vote, comment and upload image is not possible in GagBook due to lack of API.\n\
This app may involve transmitting huge amount of data when downloading images.\n\n\
Developed by Dickson."
        }
    }

    PageHeader{
        id: pageHeader
        text: "About"
    }
}
