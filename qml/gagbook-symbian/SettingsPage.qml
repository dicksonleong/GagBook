import QtQuick 1.1
import com.nokia.symbian 1.1

Page{
    id: settingsPage

    tools: ToolBarLayout{
        ToolButton{
            platformInverted: settings.whiteTheme
            iconSource: "toolbar-back"
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
                onCheckedButtonChanged: {
                    if(settingsPage.status === PageStatus.Active)
                        settings.imageSize = (checkedButton === mediumImageSizeButton ? 0 : 1)
                }

                Button{
                    id: mediumImageSizeButton
                    platformInverted: settings.whiteTheme
                    text: "Medium"
                }
                Button{
                    id: largeImageSizeButton
                    platformInverted: settings.whiteTheme
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
