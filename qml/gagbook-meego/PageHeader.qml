import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1

Item{
    id: root

    property string text
    property bool busy: false
    property bool comboboxVisible: false
    signal clicked

    height: constant.headerHeight
    width: parent.width

    Image{
        id: background
        anchors.fill: parent
        source: headerPress.pressed ? "Images/color10-meegotouch-view-header-fixed-pressed.png"
                                    : "Images/color10-meegotouch-view-header-fixed.png"
        sourceSize.width: parent.width
        sourceSize.height: parent.height
    }

    Text{
        id: mainText
        anchors{
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: busyLoader.left
            margins: constant.paddingXLarge
        }
        font.pixelSize: constant.fontSizeXLarge
        color: "white"
        elide: Text.ElideRight
        text: root.text
    }

    Loader{
        id: busyLoader
        anchors{
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: constant.paddingXLarge
        }
        sourceComponent: busy ? updatingIndicator : (comboboxVisible ? combobox : undefined )
    }

    Component{
        id: updatingIndicator

        BusyIndicator{
            platformStyle: BusyIndicatorStyle{ inverted: true }
            running: true
        }
    }

    Component{
        id: combobox

        Image{
            source: "Images/meegotouch-combobox-indicator-inverted.png"
        }
    }

    MouseArea{
        id: headerPress
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
