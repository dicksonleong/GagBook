import QtQuick 1.1
import com.nokia.meego 1.0

Item{
    id: root

    property string text: ""
    property alias checked: switchItem.checked

    width: parent.width
    height: switchItem.height + 2 * switchItem.anchors.topMargin

    Label{
        anchors{ left: parent.left; right: switchItem.left; verticalCenter: parent.verticalCenter }
        anchors.margins: constant.paddingMedium
        wrapMode: Text.Wrap
        font.bold: true
        text: root.text
    }

    Switch{
        id: switchItem
        anchors{ right: parent.right; top: parent.top; margins: constant.paddingXLarge }
        enabled: root.enabled
    }
}
