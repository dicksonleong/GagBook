import QtQuick 1.1

QtObject{
    id: constant

    property color colorHighlighted: colorLight
    property color colorLight: theme.inverted ? "#ffffff" : "#191919"
    property color colorMid: theme.inverted ? "#8c8c8c" : "#666666"
    property color colorTextSelection: "#4591ff"
    property color colorDisabled: theme.inverted ? "#444444" : "#b2b2b4"

    property int paddingSmall: 4
    property int paddingMedium: 6
    property int paddingLarge: 8
    property int paddingXLarge: 12
    property int paddingXXLarge: 16

    property int fontSizeXSmall: 20
    property int fontSizeSmall: 22
    property int fontSizeMedium: 24
    property int fontSizeLarge: 26
    property int fontSizeXLarge: 28
    property int fontSizeXXLarge: 32

    property int headerHeight: inPortrait ? 72 : 56
    property int commentsBoxWidth: inPortrait ? 320 : 570

    property url devWebSite: "http://dickson-apps.blogspot.com"
    property url sourceRepoSite: "https://github.com/dicksonleong/GagBook"
}
