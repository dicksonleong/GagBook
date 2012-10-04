import QtQuick 1.1
import com.nokia.symbian 1.1

SelectionDialog{
    id: sectionDialog

    platformInverted: settings.whiteTheme
    titleText: "Section"
    selectedIndex: settings.selectedSection
    model: ListModel{
        ListElement{ name: "Hot" }
        ListElement{ name: "Trending" }
        ListElement{ name: "Vote" }
    }
    onSelectedIndexChanged: settings.selectedSection = selectedIndex

    Component.onCompleted: open()
}
