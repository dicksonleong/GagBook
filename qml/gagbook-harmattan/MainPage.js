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

var __sectionDialogComponent = null
var __openLinkDialogComponent = null

function createSectionDialog() {
    if (!__sectionDialogComponent) __sectionDialogComponent = Qt.createComponent("SectionDialog.qml")
    var dialog = __sectionDialogComponent.createObject(mainPage)
    if (!dialog) {
        console.log("Error creating object: " + __sectionDialogComponent.errorString())
        return
    }
    dialog.accepted.connect(refresh)
}

function createOpenLinkDialog(link) {
    if (!__openLinkDialogComponent) __openLinkDialogComponent = Qt.createComponent("OpenLinkDialog.qml")
    var dialog = __openLinkDialogComponent.createObject(mainPage, { link: link })
    if (!dialog) {
        console.log("Error creating object: " + __openLinkDialogComponent.errorString())
        return
    }
}

function refresh(isAll) {
    if (isAll == undefined) isAll = true
    if (isAll) {
        gagListView.model.clear()
        nextPageId = 0
    }
    Server.getGAG(settings.selectedSection, nextPageId, onSuccess, onFailure)
    pageHeader.busy = true
}

function onSuccess(json) {
    nextPageId = json.attributes.next
    for (var i=0; i< json.images.length ; i++) {
        gagListView.model.append(json.images[i])
    }
    pageHeader.busy = false
}

function onFailure(status, statusText) {
    if (status === 0) infoBanner.alert("Server or connection error")
    else infoBanner.alert("Error: " + statusText + " (" + status + ")")
    pageHeader.busy = false
}
