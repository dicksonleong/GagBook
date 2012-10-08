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
        sectionDialog.statusChanged.connect(destroyOpenSectionDialog)
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
            return
        }
        openLinkQueryDialog.statusChanged.connect(destroyOpenLinkQueryDialog)
    }
}

function destroyOpenSectionDialog(){
    if(sectionDialog.status === DialogStatus.Closed){
        sectionDialog.destroy()
    }
}

function destroyOpenLinkQueryDialog(){
    if(openLinkQueryDialog.status === DialogStatus.Closed){
        openLinkQueryDialog.destroy()
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
