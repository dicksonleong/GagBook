function openSectionDialog(){
    if(sectionDialog) sectionDialog.open()
    else{
        var comp = Qt.createComponent("SectionDialog.qml")
        sectionDialog = comp.createObject(mainPage)
        if(!sectionDialog) {
            console.log("Error creating object: " + comp.errorString())
            return
        }
        sectionDialog.accepted.connect(refresh)
    }
}

function openOpenLinkQueryDialog(link){
    if(openLinkQueryDialog) {
        openLinkQueryDialog.link = link
        openLinkQueryDialog.open()
    }
    else{
        var comp = Qt.createComponent("OpenLinkQueryDialog.qml")
        openLinkQueryDialog = comp.createObject(mainPage, { link: link })
        if(!openLinkQueryDialog){
            console.log("Error creating object: " + comp.errorString())
        }
    }
}

function refresh(isAll){
    if(isAll == undefined) isAll = true
    if(isAll){
        gagListView.model.clear()
        nextPageId = 0
    }
    Server.getGAG(settings.selectedSection, nextPageId, onSuccess, onFailure)
    pageHeader.busy = true
}

function onSuccess(json){
    nextPageId = json.attributes.next
    for(var i=0; i< json.images.length ; i++){
        gagListView.model.append(json.images[i])
    }
    pageHeader.busy = false
}

function onFailure(status, statusText){
    if(status === 0) infoBanner.alert("Error: Server or connection error")
    else infoBanner.alert("Error: " + statusText + " (" + status + ")")
    pageHeader.busy = false
}
