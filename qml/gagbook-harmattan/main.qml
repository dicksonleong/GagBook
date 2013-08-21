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
import com.nokia.meego 1.0
import com.nokia.extras 1.1
import GagBook 1.0

PageStackWindow {
    id: appWindow

    showStatusBar: inPortrait
    initialPage: MainPage { id: mainPage }

    Constant { id: constant }

    Binding {
        target: theme
        property: "inverted"
        value: !settings.whiteTheme
    }

    InfoBanner {
        id: infoBanner
        topMargin: showStatusBar ? 40 : 8

        function alert(text) {
            infoBanner.text = text
            infoBanner.show()
        }
    }

    GagManager {
        id: gagManager
        model: GagModel {}
        onRefreshFailure: infoBanner.alert("Error: " + errorMessage)
    }

    ListModel {
        id: sectionModel
        ListElement { text: "Hot" }
        ListElement { text: "Trending" }
        ListElement { text: "Fresh" }
        ListElement { text: "Cute" }
        ListElement { text: "Geeky" }
        ListElement { text: "GIF" }
    }

    Component.onCompleted: gagManager.refresh(GagManager.RefreshAll)
}
