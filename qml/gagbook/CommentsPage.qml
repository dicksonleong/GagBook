import QtQuick 1.1
import com.nokia.meego 1.0
import QtWebKit 1.0

Page {
    id: commentsPage

    property string gagURL

    tools: ToolBarLayout{
        ToolIcon{
            platformIconId: "toolbar-back"
            onClicked: pageStack.pop()
        }
    }

    Flickable{
        id: webViewFlickable
        anchors{ top: pageHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        contentHeight: commentsWebView.height * commentsWebView.scale

        Behavior on contentY{ SmoothedAnimation{ velocity: 2500 } }

        WebView{
            id: commentsWebView
            width: commentsBox.width + 20
            scale: webViewFlickable.width / width
            transformOrigin: Item.TopLeft

            javaScriptWindowObjects: QtObject{
                id: commentsBox
                WebView.windowObjectName: "qml"

                property string gagURL: commentsPage.gagURL
                property int width: inPortrait ? 320 : 570
            }

            onLoadStarted: pageHeader.busy = true
            onLoadFailed: pageHeader.busy = false
            onLoadFinished: pageHeader.busy = false

            Connections{ target: appWindow; onInPortraitChanged: commentsWebView.reload.trigger() }
        }
    }

    ScrollDecorator{ flickableItem: webViewFlickable }

    PageHeader{
        id: pageHeader
        text: "Comments"
        onClicked: webViewFlickable.contentY = 0
    }

    Component.onCompleted: commentsWebView.url = "comments.html"
}
