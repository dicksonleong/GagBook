import QtQuick 1.1

QtObject{
    id: settings

    property int selectedSection: settingsStorage.read("selectionSection", 0)
    onSelectedSectionChanged: settingsStorage.store("selectionSection", selectedSection)

    property int imageSize: settingsStorage.read("imageSize", 0)
    onImageSizeChanged: settingsStorage.store("imageSize", imageSize)

    property bool whiteTheme: settingsStorage.read("whiteTheme", false)
    onWhiteThemeChanged: settingsStorage.store("whiteTheme", whiteTheme)
}
