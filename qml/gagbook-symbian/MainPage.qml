import QtQuick 1.1
import com.nokia.symbian 1.1
import "infinigag.js" as Server
import "MainPage.js" as Script

Page {
    id: mainPage

    property Item sectionDialog: null
    property Item openLinkQueryDialog: null
    property int nextPageId: 0

    tools: ToolBarLayout{
        ToolButton{
            platformInverted: settings.whiteTheme
            iconSource: "Images/close_stop" + (platformInverted ? "_inverted.svg" : ".svg")
            onClicked: Qt.quit()
        }
        ToolButton{
            platformInverted: settings.whiteTheme
            iconSource: "Images/instant_messenger_chat" + (platformInverted ? "_inverted.svg" : ".svg")
            enabled: gagListView.count > 0
            opacity: enabled ? 1 : 0.25
            onClicked: {
                var prop = {gagURL: gagListView.model.get(gagListView.currentIndex).url}
                pageStack.push(Qt.resolvedUrl("CommentsPage.qml"), prop)
            }
        }
        ToolButton{
            platformInverted: settings.whiteTheme
            iconSource: "Images/internet" + (platformInverted ? "_inverted.svg" : ".svg")
            enabled: gagListView.count > 0
            opacity: enabled ? 1 : 0.25
            onClicked: Script.openOpenLinkQueryDialog(gagListView.model.get(gagListView.currentIndex).url)
        }
        ToolButton{
            platformInverted: settings.whiteTheme
            iconSource: "toolbar-menu"
            onClicked: mainMenu.open()
        }
    }

    Menu{
        id: mainMenu
        platformInverted: settings.whiteTheme

        MenuLayout{
            MenuItem{
                platformInverted: settings.whiteTheme
                text: "Refresh section"
                enabled: !pageHeader.busy
                onClicked: Script.refresh()
            }
            MenuItem{
                platformInverted: settings.whiteTheme
                text: "Save image"
                enabled: gagListView.count > 0
                onClicked: {
                    var filePath = gagListView.currentItem.saveImage()
                    if(filePath) infoBanner.alert("Image saved in " + filePath)
                    else infoBanner.alert("Failed to save image")
                }
            }
            MenuItem{
                platformInverted: settings.whiteTheme
                text: "Settings"
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
            MenuItem{
                platformInverted: settings.whiteTheme
                text: "About GagBook"
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }
    }

    ListView{
        id: gagListView
        anchors{ top: pageHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        model: ListModel{}
        boundsBehavior: Flickable.DragOverBounds
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        delegate: GagDelegate{}
        interactive: count === 0 || !currentItem.allowDelegateFlicking || moving
        onAtXEndChanged: if(atXEnd && count > 0 && !pageHeader.busy) Script.refresh(false)
    }

    PageHeader{
        id: pageHeader
        text: {
            switch(settings.selectedSection){
            case 0: return "Hot"
            case 1: return "Trending"
            case 2: return "Vote"
            default: return ""
            }
        }
        comboboxVisible: true
        onClicked: Script.openSectionDialog()
    }

    Component.onCompleted: Script.refresh()
}
