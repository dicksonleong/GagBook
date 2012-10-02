import QtQuick 1.1
import com.nokia.symbian 1.1

QueryDialog{
    id: openLinkQueryDialog

    property string link: ""

    platformInverted: settings.whiteTheme
    titleIcon: "Images/internet.svg"
    titleText: "Open Link"
    message: "Do you want to open the following link in web browser?\n\n" + link + "\n"
    acceptButtonText: "Yes"
    rejectButtonText: "No"

    onAccepted: {
        Qt.openUrlExternally(link)
        infoBanner.alert("Launching web browser...")
    }

    Component.onCompleted: open()
}
