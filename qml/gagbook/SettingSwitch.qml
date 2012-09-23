import QtQuick 1.1
import com.nokia.meego 1.0

Item{
    id: root

    property string text: ""
    property alias checked: switchItem.checked

    width: parent.width
    height: switchItem.height + 2 * switchItem.anchors.topMargin

    Label{
        anchors.left: parent.left
        anchors.right: switchItem.left
        anchors.margins: constant.paddingMedium
        anchors.verticalCenter: parent.verticalCenter
        wrapMode: Text.Wrap
        font.bold: true
        text: root.text
    }

    Switch{
        id: switchItem
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: constant.paddingXLarge
        anchors.rightMargin: constant.paddingXLarge
        enabled: root.enabled
    }
}
