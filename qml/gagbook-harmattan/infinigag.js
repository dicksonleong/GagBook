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

var INFINIGAG_URL = "http://infinigag.com/api.json"
var USER_AGENT = "GagBook/" + APP_VERSION + " (Nokia; Qt; MeeGo/1.2; Harmattan)"

function getGAG(section, page, onSuccess, onFailure) {
    var sectionString = ""
    switch (section) {
    case 0: sectionString = "hot"; break;
    case 1: sectionString = "trending"; break;
    case 2: sectionString = "vote"; break;
    default: throw new Error("Invalid section: " + section)
    }

    var requestUrl = INFINIGAG_URL + "?section=" + sectionString + (page ? "&page=" + page : "")
    var request = new XMLHttpRequest()

    request.open("GET", requestUrl)
    request.setRequestHeader("User-Agent", USER_AGENT)

    request.onreadystatechange = function() {
        if (request.readyState === XMLHttpRequest.DONE) {
            if (request.status === 200) {
                if (request.responseText) onSuccess(JSON.parse(request.responseText))
                else onFailure(-1, "Empty response")
            }
            else onFailure(request.status, request.statusText)
        }
    }
    request.send()
}
