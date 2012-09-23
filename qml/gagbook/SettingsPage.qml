import QtQuick 1.1
import com.nokia.meego 1.0

Page{
    id: settingsPage

    tools: ToolBarLayout{
        ToolIcon{
            platformIconId: "toolbar-back"
            onClicked: pageStack.pop()
        }
    }

    Column{
        id: settingsColumn
        anchors{ left: parent.left; right: parent.right; top: pageHeader.bottom; topMargin: constant.paddingMedium }
        height: childrenRect.height
        spacing: constant.paddingMedium

        SettingSwitch{
            text: "White Theme"
            checked: settings.whiteTheme
            onCheckedChanged: settings.whiteTheme = checked
        }

        Item{
            width: parent.width
            height: imageSizeText.paintedHeight + imageSizeButtonRow.height + imageSizeButtonRow.anchors.topMargin

            Text{
                id: imageSizeText
                anchors{ left: parent.left; top: parent.top; leftMargin: constant.paddingMedium }
                font.pixelSize: constant.fontSizeMedium
                font.bold: true
                color: constant.colorLight
                text: "Image Size"
            }

            ButtonRow{
                id: imageSizeButtonRow
                anchors{
                    top: imageSizeText.bottom
                    margins: constant.paddingMedium
                    left: parent.left
                    right: parent.right
                }
                checkedButton: settings.imageSize === 0 ? mediumImageSizeButton : largeImageSizeButton
                onCheckedButtonChanged: settings.imageSize = (checkedButton === mediumImageSizeButton ? 0 : 1)

                Button{
                    id: mediumImageSizeButton
                    text: "Medium"
                }
                Button{
                    id: largeImageSizeButton
                    text: "Large"
                }
            }
        }
    }

    PageHeader{
        id: pageHeader
        text: "Settings"
    }
}
