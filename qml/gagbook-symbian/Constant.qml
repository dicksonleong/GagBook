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

QtObject {
    id: constant

    property color colorHighlighted: colorLight
    property color colorLight: settings.whiteTheme ? platformStyle.colorNormalLightInverted
                                                   : platformStyle.colorNormalLight
    property color colorMid: settings.whiteTheme ? platformStyle.colorNormalMidInverted
                                                 : platformStyle.colorNormalMid
    property color colorTextSelection: settings.whiteTheme ? platformStyle.colorTextSelectionInverted
                                                           : platformStyle.colorTextSelection
    property color colorDisabled: settings.whiteTheme ? platformStyle.colorDisabledMidInverted
                                                      : platformStyle.colorDisabledLight

    property int paddingSmall: platformStyle.paddingSmall
    property int paddingMedium: platformStyle.paddingMedium
    property int paddingLarge: platformStyle.paddingLarge
    property int paddingXLarge: platformStyle.paddingLarge + platformStyle.paddingSmall
    property int paddingXXLarge: platformStyle.paddingLarge + platformStyle.paddingMedium

    property int fontSizeXSmall: platformStyle.fontSizeSmall - 2
    property int fontSizeSmall: platformStyle.fontSizeSmall
    property int fontSizeMedium: platformStyle.fontSizeMedium
    property int fontSizeLarge: platformStyle.fontSizeLarge
    property int fontSizeXLarge: platformStyle.fontSizeLarge + 2
    property int fontSizeXXLarge: platformStyle.fontSizeLarge + 4

    property int headerHeight: inPortrait ? 55 : 45

    property url devWebSite: "http://dickson-apps.blogspot.com"
    property url sourceRepoSite: "https://github.com/dicksonleong/GagBook"
}
