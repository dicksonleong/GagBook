import QtQuick 2.0
import Sailfish.Silica 1.0
import GagBook 1.0
CoverBackground {
    Cover {
        id: cover
        anchors.fill: parent

        GagModel {
            id: gagModel
            manager: gagbookManager
            onRefreshFailure: {
                console.log("refresh failed");
            }
        }

        SlideshowView {
            id: slideView
            objectName: "slideshowView"
            anchors.fill: parent
            itemWidth: parent.width
            clip:true

            model: gagModel
            delegate: Rectangle {
                anchors.fill: parent
                color: Theme.primaryColor
                Image {
                    id: image
                    property int currentIndex: 0
                    anchors.fill: parent
                    source: model.imageUrl
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
        CoverActionList {
            id: coverAction

            CoverAction {
                iconSource: "image://theme/icon-cover-next"
                onTriggered: {
                    slideView.incrementCurrentIndex();
                }
            }

            CoverAction {
                iconSource: "image://theme/icon-cover-refresh"
                onTriggered: {
                    console.log("cover refresh");
                    gagModel.refresh(GagModel.RefreshAll);
                }
            }
        }

        Timer {
            id: slideTimer
            interval: 30000
            running:cover.status == Cover.Active
            repeat: true
            onTriggered: {
                console.log("cover timer triggered, swiping pic");
                if (slideView.count > 0)
                    slideView.incrementCurrentIndex();
                else
                    gagModel.refresh(GagModel.RefreshAll);
            }

            onRunningChanged: {
                console.log("cover running status changed to: " + cover.status);
                if (cover.status == Cover.Active)
                    gagModel.refresh(GagModel.RefreshAll);
            }
        }
    }
}
