import QtQuick 1.1

QtObject{
    id: settings

    property int selectedSection: settingsStorage.read("general/selectionSection", 0)
    onSelectedSectionChanged: settingsStorage.store("general/selectionSection", selectedSection)

    property int imageSize: settingsStorage.read("general/imageSize", 0)
    onImageSizeChanged: settingsStorage.store("general/imageSize", imageSize)

    property bool whiteTheme: settingsStorage.read("general/whiteTheme", false)
    onWhiteThemeChanged: {
        theme.inverted = !whiteTheme
        settingsStorage.store("general/whiteTheme", whiteTheme)
    }
}
