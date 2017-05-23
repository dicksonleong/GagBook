import QtQuick 2.0
import Sailfish.Silica 1.0

Page {

    id: commentsPage

    property string gagURL
    readonly property string rootUrl: "https://comment-cdn.9gag.com/v1/cacheable/comment-list.json?appId=a_dd8f2b7d304a10edaf6f29517ea0ca4100a43d1b&url=%1&count=%2&level=2&order=score&mentionMapping=true"
    property int nb_comment : 20


    SilicaListView {

        id: flick
        width: 600; height: 800
        anchors.fill: parent
        spacing: Theme.paddingMedium

        header: PageHeader {
            title: "Comments"
        }

        model: ListModel {
            id:listComment
            Component.onCompleted: {
                var req = new XMLHttpRequest()
                var gag_url = gagURL.replace("\\","");
                gag_url = gag_url.replace(/^https:\/\//i, 'http://');
                var url = rootUrl.arg(gag_url).arg(nb_comment);
                req.open("GET", url, true);


                // Send the proper header information along with the request
                //req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
                //req.setRequestHeader("Content-length", params.length);
                //req.setRequestHeader("Connection", "close");
                var objectArray;
                var objectArray1;
                req.onreadystatechange = function() { // Call a function when the state changes.
                    console.log(req.readyState);
                            if (req.readyState == 4) {
                                //if (req.status == 200) {

                                    //objectArray1 = JSON.parse(req.responseText);

                                     objectArray = null;
                                     objectArray = eval('new Object(' + req.responseText + ')');

                                    console.log(objectArray.status);
                                    console.log(objectArray.payload.url);
                                    console.log(objectArray.payload.total);
                                    var comments = objectArray.payload.comments;
                                   for (var i=0; i< comments.length; i++)  {
                                       var hasURL = comments[i].richtext;
                                       var lenght_hasURL = hasURL.length;
                                       if (hasURL.substring(0, 5) != "[url]"){
                                           var str = comments[i].richtext;
                                           var map =
                                               {
                                                   '&amp;': '&',
                                                   '&lt;': '<',
                                                   '&gt;': '>',
                                                   '&quot;': '"',
                                                   '&#039;': "'"
                                               };
                                           str = str.replace(/&amp;|&lt;|&gt;|&quot;|&#039;/g, function(m) {return map[m];});

                                           if (str.match(/(\r\n|\n|\r)/g)){

                                               str = str.replace(/(\r\n|\n|\r)/g,"<br />");
                                           }else{
                                            str = str.replace(/(.{70})/g, "$&"+"<br>");
                                           }
                                           append({"user": comments[i].user.displayName, "comm":str,"like": comments[i].likeCount,"imgURL":""})

                                           // Handle children comment
                                           if (comments[i].children[0]){

                                               var child_comment = comments[i].children[0];
                                               var hasURL = child_comment.richtext;
                                               var lenght_hasURL = hasURL.length;
                                               if (hasURL.substring(0, 5) != "[url]"){
                                                   var str = child_comment.richtext;
                                                   var map =
                                                       {
                                                           '&amp;': '&',
                                                           '&lt;': '<',
                                                           '&gt;': '>',
                                                           '&quot;': '"',
                                                           '&#039;': "'"
                                                       };
                                                   str = str.replace(/&amp;|&lt;|&gt;|&quot;|&#039;/g, function(m) {return map[m];});

                                                   if (str.match(/(\r\n|\n|\r)/g)){

                                                       str = str.replace(/(\r\n|\n|\r)/g,"<br />");
                                                   }else{
                                                    str = str.replace(/(.{70})/g, "$&"+"<br>");
                                                   }
                                               append({"user": child_comment.user.displayName, "comm":str,"like": child_comment.likeCount,"imgURL":"","isChild":"yes"})
                                               }else{
                                                    append({"user": child_comment.user.displayName, "comm":"","like": child_comment.likeCount,"imgURL":hasURL.substring(5, lenght_hasURL -5),"isChild":"yes"})
                                               }
                                           }

                                       }else{

                                           append({"user": comments[i].user.displayName, "comm":"","like": comments[i].likeCount,"imgURL":hasURL.substring(5, lenght_hasURL -5)})
                                           // Handle children comment
                                           if (comments[i].children[0]){

                                                   var child_comment = comments[i].children[0];
                                                   var hasURL = child_comment.richtext;
                                                   var lenght_hasURL = hasURL.length;
                                                   if (hasURL.substring(0, 5) != "[url]"){
                                                       var str = child_comment.richtext;
                                                       var map =
                                                           {
                                                               '&amp;': '&',
                                                               '&lt;': '<',
                                                               '&gt;': '>',
                                                               '&quot;': '"',
                                                               '&#039;': "'"
                                                           };
                                                       str = str.replace(/&amp;|&lt;|&gt;|&quot;|&#039;/g, function(m) {return map[m];});

                                                       if (str.match(/(\r\n|\n|\r)/g)){

                                                           str = str.replace(/(\r\n|\n|\r)/g,"<br />");
                                                       }else{
                                                        str = str.replace(/(.{70})/g, "$&"+"<br>");
                                                       }
                                                        append({"user": child_comment.user.displayName, "comm":str,"like": child_comment.likeCount,"imgURL":"","isChild":"yes"})
                                                   }else{
                                                        append({"user": child_comment.user.displayName, "comm":"","like": child_comment.likeCount,"imgURL":hasURL.substring(5, lenght_hasURL -5),"isChild":"yes"})
                                                   }
                                               }
                                       }

                                    }
                               // } else {
                               //      console.log("error: " + req.status)
                              //       append({"user":"error", "comment":req.status})
                              //  }
                            }
                        }
                req.send('');


            }
        }
        HorizontalScrollDecorator {}
        VerticalScrollDecorator {}

        delegate: ListItem {


            width: ListView.view.width
            contentHeight: Theme.itemSizeExtraLarge



            Text {


                id: userLabel
                text: "<i><b>"+model.user+ "</b></i> · " + model.like +" points <br/> " + model.comm + "<br/>"
                font.pixelSize: Theme.fontSizeExtraSmall * 3/4
                color: Theme.primaryColor
                wrapMode: Text.Wrap
                width: 700
                height: 300

            }
           // Image{
            AnimatedImage {
                id: gagImage
                asynchronous: true
                anchors.fill: parent
                cache: true
                fillMode: Image.PreserveAspectFit
                //sourceSize.height: QMLUtils.IMAGE_MAX_HEIGHT
                source: model.imgURL

                onStatusChanged: {
                    if (status == Image.Ready) {
                        loadedAnimation.stop()
                        loadedAnimation.start()
                    }
                }



                NumberAnimation {
                    id: loadedAnimation
                    target: gagImage
                    property: "opacity"
                    duration: 250
                    from: 0; to: 1
                    easing.type: Easing.InOutQuad
                }
            } // Image End
            Loader {
                anchors.centerIn: parent
                sourceComponent: {
                    switch (gagImage.status) {
                    case Image.Loading: return loadingIndicator
                    case Image.Error: return failedLoading
                    default: return undefined
                    }
                }

                Component {
                    id: loadingIndicator

                    BusyIndicator {
                        id: busyIndicator
                        running: true
                        size: BusyIndicatorSize.Large
                    }
                }

                Component { id: failedLoading; Label { text: "Error loading image" } }
            }

        }



    }

}
