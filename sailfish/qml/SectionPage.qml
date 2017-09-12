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
import harbour.gagbook.Core 1.0

Page {
    id: sectionPage

    property GagModel gagModel

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: appSettings.sections

        header: PageHeader { title: "Section" }

        PullDownMenu {
            MenuItem {
                text: qsTr("Add section")
                onClicked: sectionPanel.open = true
            }
        }

        delegate:
            BackgroundItem {
            id: bgdelegate
            width: parent.width
            height: menuOpen ? contextMenu.height + simpleListItem.height : simpleListItem.height
            property Item contextMenu
            property bool menuOpen: contextMenu != null && contextMenu.parent === bgdelegate

            function remove() {
                var removal = removalComponent.createObject(bgdelegate)
                removal.execute(simpleListItem,qsTr("Deleting ") + modelData, function() { appSettings.sections.splice(appSettings.sections.indexOf(modelData), 1) })
            }

            SimpleListItem {
                id: simpleListItem
                selected: gagModel.selectedSection == index
                text: modelData

                function showContextMenu() {
                    if (!contextMenu)
                        contextMenu = myMenu.createObject(listView)
                    contextMenu.show(bgdelegate)
                }

                onClicked: {
                    gagModel.selectedSection = index;
                    gagModel.refresh(GagModel.RefreshAll);
                    pageStack.navigateBack();
                }

                onPressAndHold: showContextMenu()

            }
            Component {
                id: removalComponent
                RemorseItem {
                    id: remorse
                    onCanceled: destroy()
                }
            }

            Component {
                id: myMenu
                ContextMenu {
                    MenuItem {
                        text: qsTr("Delete")
                        onClicked: {
                            bgdelegate.remove();
                        }
                    }
                }
            }
        } // Background Item End
    }

    DockedPanel {
        id: sectionPanel

        width: sectionPage.isPortrait ? parent.width : Theme.itemSizeExtraLarge + Theme.paddingLarge
        height: sectionPage.isPortrait ? Theme.itemSizeExtraLarge + Theme.paddingLarge : parent.height
        modal: true

        dock: sectionPage.isPortrait ? Dock.Bottom : Dock.Right
        onOpenChanged: {
            if (open) inputName.forceActiveFocus();
            else sectionPage.forceActiveFocus();
        }

        TextField {
            id: inputName
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width - (Theme.paddingLarge * 2)
            placeholderText: qsTr("Enter Sectioname")
            label: "Section"
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            // Only allow Enter key to be pressed when text has been entered
            EnterKey.enabled: text.length > 0
            EnterKey.onClicked: {
                appSettings.sections.push(text)
                sectionPanel.open = false
                Qt.inputMethod.hide();
                sectionPage.forceActiveFocus();
            }
        }

    }

}
