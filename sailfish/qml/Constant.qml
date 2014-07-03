/*
 * Copyright (c) 2014 Dickson Leong.
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

import QtQuick 2.0
import Sailfish.Silica 1.0

QtObject {
    id: constant

    readonly property color colorLight: Theme.primaryColor
    readonly property color colorMid: Theme.secondaryColor
    readonly property color colorDisabled: Qt.darker(colorMid, 1.5)

    readonly property int paddingSmall: Theme.paddingSmall
    readonly property int paddingMedium: Theme.paddingMedium
    readonly property int paddingLarge: Theme.paddingLarge
    readonly property int paddingXLarge: paddingLarge + paddingSmall

    readonly property int fontSizeXSmall: Theme.fontSizeExtraSmall
    readonly property int fontSizeSmall: Theme.fontSizeSmall
    readonly property int fontSizeMedium: Theme.fontSizeMedium
    readonly property int fontSizeLarge: Theme.fontSizeLarge
    readonly property int fontSizeXLarge: Theme.fontSizeExtraLarge
    readonly property int fontSizeXXLarge: Theme.fontSizeHuge
}
