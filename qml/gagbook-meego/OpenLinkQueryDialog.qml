import QtQuick 1.1
import com.nokia.meego 1.0

QueryDialog{
    id: openLinkQueryDialog

    property string link: ""

    icon: "image://theme/icon-l-browser"
    titleText: "Open Link"
    message: "Do you want to open the following link in web browser?\n\n" + link
    acceptButtonText: "Yes"
    rejectButtonText: "No"

    onAccepted: {
        Qt.openUrlExternally(link)
        infoBanner.alert("Launching web browser...")
    }

    Component.onCompleted: open()
}
