import QtQuick 1.1
import com.nokia.meego 1.0

SelectionDialog{
    id: sectionDialog

    titleText: "Section"
    selectedIndex: selectedSection
    model: ListModel{
        ListElement{ name: "Hot" }
        ListElement{ name: "Trending" }
        ListElement{ name: "Vote" }
    }
    onSelectedIndexChanged: selectedSection = selectedIndex

    Component.onCompleted: open()
}
