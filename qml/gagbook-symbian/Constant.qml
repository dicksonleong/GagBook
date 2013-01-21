/*
 * Copyright (c) 2012-2013 Dickson Leong.
 * All rights reserved.
 *
 * This file is part of GagBook.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of GagBook nor the names of its contributors may be
 *       used to endorse or promote products derived from this software without
 *       specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
