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

Page {
    id: commentsPage

    property string gagURL
    property string appId: "a_dd8f2b7d304a10edaf6f29517ea0ca4100a43d1b"
    readonly property string rootUrl: "https://m." + gagURL.slice(8) + "#comment"
    property string apiCallUrl: "http://comment-cdn.9gag.com/v1/topComments.json?appId=" +
                                appId +
                                "&urls=" + gagURL + "&commentL1=5&commentL2=3&pretty=0"
    /*readonly property string rootUrl: "https://comment.9gag.com/comment/list?url=%1" +
                             "&appId=a_dd8f2b7d304a10edaf6f29517ea0ca4100a43d1b&readOnly=1"*/

    // Not working anymore as it seems
//    function getComments() {
//        var xhr = new XMLHttpRequest();
//        xhr.open("GET",apiCallUrl,true);
//        xhr.onreadystatechange = function() {
//            if (xhr.readyState === 4) {
//                if (xhr.status === 200) {
//                    var jsonObject = eval(xhr.responseText);
//                    console.log("Payload Data: " + jsonObject.payload.data[0].comments);
//                } else {
//                    console.log("responseText", xhr.responseText);
//                }
//            }
//        }
//        xhr.send();
//    }

    SilicaWebView {
        anchors.fill: parent
        header: PageHeader { title: "Comments" }
        url: rootUrl

        experimental.overview: true
        property variant devicePixelRatio: {//1.5
            if (Screen.width <= 540) return 1.5;
            else if (Screen.width > 540 && Screen.width <= 768) return 2.0;
            else if (Screen.width > 768) return 3.0;
        }
        experimental.customLayoutWidth: commentsPage.width / devicePixelRatio

        experimental.userScripts: [
                    Qt.resolvedUrl("devicePixelRatioHack.js")
        ]

        PullDownMenu {
            MenuItem {
                text: "Go Back"
                onClicked: pageStack.pop();
            }
        }

        VerticalScrollDecorator {}

//        Component.onCompleted: {
//            console.debug("URL: " + rootUrl)
//            getComments();
//        }
    }
}
