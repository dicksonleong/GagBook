import QtQuick 1.1
import com.nokia.symbian 1.1
import com.nokia.extras 1.1

PageStackWindow {
    id: appWindow

    platformInverted: settings.whiteTheme
    initialPage: MainPage{ id: mainPage }

    Constant{ id: constant }
    Settings{ id: settings }

    InfoBanner{
        id: infoBanner
        platformInverted: settings.whiteTheme

        function alert(text){
            infoBanner.text = text
            infoBanner.open()
        }
    }
}
