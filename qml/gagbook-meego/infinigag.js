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

//  This app use InfiniGAG API <http://infinigag.com/api/>

.pragma library

var INFINIGAG_URL = "http://liquid8002.untergrund.net/infinigag/"
var USER_AGENT = "GagBook/0.2 (Nokia; Qt; MeeGo Harmattan)"

function getGAG(section, page, onSuccess, onFailure) {
    var sectionString = ""
    switch(section){
    case 0: sectionString = "hot"; break;
    case 1: sectionString = "trending"; break;
    case 2: sectionString = "vote"; break;
    default: throw new Error("Invalid section: " + section)
    }

    var requestUrl = INFINIGAG_URL + "?section=" + sectionString + (page ? "&page=" + page : "")
    var request = new XMLHttpRequest()

    request.open("GET", requestUrl)
    request.setRequestHeader("User-Agent", USER_AGENT)

    request.onreadystatechange = function(){
                if(request.readyState === XMLHttpRequest.DONE){
                    if(request.status === 200){
                        if(request.responseText) onSuccess(JSON.parse(request.responseText))
                        else onFailure(-1, "Empty response")
                    }
                    else onFailure(request.status, request.statusText)
                }
            }
    request.send()
}
