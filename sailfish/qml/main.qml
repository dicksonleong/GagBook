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
import QtSystemInfo 5.0
import GagBook 1.0

ApplicationWindow {
    id: appWindow

    //showStatusBar: inPortrait
    initialPage: MainPage { id: mainPage }
    cover: CoverBackground {
        CoverPlaceholder {
            text: "GagBook"
            icon.source: "../harbour-gagbook.png"
        }
    }

    Constant { id: constant }

    GagBookManager {
        id: gagbookManager
        settings: AppSettings { id: appSettings }

    }

    Component.onCompleted: {
        //check connectivity, show error page if no connection
        checkNetworkAvailabilty();
    }

    function checkNetworkAvailabilty() {
        console.log(netInfo.currentNetworkMode);
        if (netInfo.currentNetworkMode == NetworkInfo.NoNetworkAvailable) {
            console.log("no internet connection or 9gag connection available");
            mainPage.state = "noConnection";
        }
        else {
            console.log("Internet available, continuing...");
            mainPage.state = "hasConnection";
        }
    }

    function networkStatusChanged() {
        console.log("Network status changed to: " + netInfo);
    }

    NetworkInfo {
        id: netInfo
    }

    Connections {
        target: netInfo
        onNetworkStatusChanged: networkStatusChanged()
    }
}
