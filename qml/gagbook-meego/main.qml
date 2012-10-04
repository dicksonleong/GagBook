import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1

PageStackWindow {
    id: appWindow

    showStatusBar: inPortrait
    initialPage: MainPage{ id: mainPage }

    Constant{ id: constant }
    Settings{ id: settings }

    InfoBanner{
        id: infoBanner
        topMargin: showStatusBar ? 40 : 8

        function alert(text){
            infoBanner.text = text
            infoBanner.show()
        }
    }

    Component.onCompleted: theme.inverted = !settings.whiteTheme
}
