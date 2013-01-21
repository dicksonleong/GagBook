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

var THUMB_IMAGE_URL = "http://d24w6bsrhbeh9d.cloudfront.net/photo/%1_220x145.jpg"
var SMALL_IMAGE_URL = "http://d24w6bsrhbeh9d.cloudfront.net/photo/%1_460s.jpg"
var BIG_IMAGE_URL = "http://d24w6bsrhbeh9d.cloudfront.net/photo/%1_700b.jpg"

var __sectionDialogComponent = null
var __openLinkDialogComponent = null

function createSectionDialog() {
    if (!__sectionDialogComponent) __sectionDialogComponent = Qt.createComponent("SectionDialog.qml")
    var dialog = __sectionDialogComponent.createObject(mainPage)
    if (!dialog) {
        console.log("Error creating object: " + __sectionDialogComponent.errorString())
        return
    }
    dialog.accepted.connect(refreshAll)
}

function createOpenLinkDialog(link) {
    if (!__openLinkDialogComponent) __openLinkDialogComponent = Qt.createComponent("OpenLinkDialog.qml")
    var dialog = __openLinkDialogComponent.createObject(mainPage, { link: link })
    if (!dialog) console.log("Error creating object: " + __openLinkDialogComponent.errorString())
}

function refreshAll() {
    gagListView.model.clear()
    nextPageId = 0
    refreshOlder()
}

function refreshOlder() {
    Server.getGAG(settings.selectedSection, nextPageId, onSuccess, onFailure)
    pageHeader.busy = true
}

function onSuccess(json) {
    nextPageId = json.attributes.next
    var imagesArray = json.images
    imagesArray.forEach(settings.selectedSection === 2 ? __appendVoteImage : __appendImage)
    pageHeader.busy = false
}

function onFailure(status, statusText) {
    if (status === 0) infoBanner.alert("Server or connection error")
    else infoBanner.alert("Error: " + statusText + " (" + status + ")")
    pageHeader.busy = false
}

function __appendImage(imageObject) {
    gagListView.model.append(imageObject)
}

// The url for images in Vote section is broken, so have to use hardcoded url
function __appendVoteImage(imageObject) {
    var idStr = imageObject.id.toString()
    imageObject.image.thumb = THUMB_IMAGE_URL.arg(idStr)
    imageObject.image.small = SMALL_IMAGE_URL.arg(idStr)
    imageObject.image.big = BIG_IMAGE_URL.arg(idStr)
    gagListView.model.append(imageObject)
}
