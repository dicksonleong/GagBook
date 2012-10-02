import QtQuick 1.1
import com.nokia.meego 1.0

Item{
    id: root

    property string text: ""
    property alias checked: switchItem.checked

    width: parent.width
    height: switchItem.height + 2 * switchItem.anchors.topMargin

    Text{
        anchors{ left: parent.left; right: switchItem.left; verticalCenter: parent.verticalCenter }
        anchors.margins: constant.paddingMedium
        color: constant.colorLight
        font.pixelSize: constant.fontSizeMedium
        wrapMode: Text.Wrap
        text: root.text
    }

    Switch{
        id: switchItem
        anchors{ right: parent.right; top: parent.top; margins: constant.paddingXLarge }
        enabled: root.enabled
    }
}
