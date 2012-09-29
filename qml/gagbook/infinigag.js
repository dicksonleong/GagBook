
//  This app use InfiniGAG API <http://infinigag.com/api/>

.pragma library

var INFINIGAG_URL = "http://liquid8002.untergrund.net/infinigag/"
var USER_AGENT = "GagBook/0.0.5 (Nokia; Qt; MeeGo Harmattan)"

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

//function getGAGComments(url){
//    var fql = "\
//SELECT fromid, text, time, likes, username, comments \ // username always returns "Anonymous User" :(
//FROM comment \
//WHERE object_id IN \
//(SELECT comments_fbid \
//FROM link_stat \
//WHERE url ='" + url + "')\
//ORDER BY likes DESC"

//    var requestUrl = "https://graph.facebook.com/fql?q=" + encodeURIComponent(fql)
//    var request = new XMLHttpRequest()
//    request.open("GET", requestUrl)
//    request.setRequestHeader("User-Agent", USER_AGENT)

//    request.onreadystatechange = function (){
//                if(request.readyState === XMLHttpRequest.DONE){
//                    if(request.status === 200){
//                        console.log("HEADER-----------\n" + request.getAllResponseHeaders())
//                        console.log("RESOPONSE-----------\n" + request.responseText)
//                    }
//                    else console.log(request.status)
//                }
//            }
//    request.send()
//    console.log(requestUrl)
//}
