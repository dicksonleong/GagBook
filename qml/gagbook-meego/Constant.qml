/*
    Copyright (C) 2012  Dickson Leong
    This file is part of GagBook.

    GagBook is free software: you can redistribute it and/or modify it
    under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. If not, see http://www.gnu.org/licenses/.
*/

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

    property url devWebSite: "http://dickson-apps.blogspot.com"
    property url sourceRepoSite: "https://github.com/dicksonleong/GagBook"
}
