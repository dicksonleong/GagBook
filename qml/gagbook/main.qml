import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1

PageStackWindow {
    id: appWindow

    // TODO: QSetting
    property int selectedSection: 0 // 0 = hot, 1 = trending, 2 = vote
    property int imageSize: 0 // 0 = medium, 1 = large

    showStatusBar: inPortrait
    initialPage: MainPage{ id: mainPage }

    Constant{ id: constant }

    InfoBanner{
        id: infoBanner
        topMargin: showStatusBar ? 40 : 8

        function alert(text){
            infoBanner.text = text
            infoBanner.show()
        }
    }

    Component.onCompleted: theme.inverted = true
}
