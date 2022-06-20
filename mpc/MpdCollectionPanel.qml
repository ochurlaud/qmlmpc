/*
 * Copyright 2015, Maarten Sebregts <maartensebregts AT gmail DOT com>
 *
 * This file is part of qmlmpc.
 *
 * qmlmpc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * qmlmpc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with qmlmpc. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick
import QtQuick.Controls

Pane {
    id: root

    signal itemClicked(string type, string itemName)
    property var model;
    function loadModel() {}

    anchors.fill: parent

    Column {
        width: parent.width
        height: parent.height

        ScrollView {
            width: parent.width
            height: parent.height
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            ListView {
                id: listView
                anchors.fill: parent
                model: root.model
                delegate: ItemDelegate {
                    width: listView.width
                    property bool selected: mpdConnector.collectionModel.isSelected(index)
                    Connections {
                        target: mpdConnector.collectionModel
                        function onSelectionChanged() {
                            selected = mpdConnector.collectionModel.isSelected(index)
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        Connections {
                            /*
                            function browse(path, type) {
                                parentList.push(path)
                                isFirstPanel = false
                                if (type === "Directory") {
                                    mpdConnector.listDirectory(path)
                                } else if (type === "Artist") {
                                    mpdConnector.listAlbums(path)
                                } else if (type === "Album") {
                                    if (parentList.length > 1) {
                                        mpdConnector.listSongsByArtistAndAlbum(parentList[parentList.length - 2],
                                                                               path)
                                    }
                                } else if (type === "Playlist") {
                                    mpdConnector.getPlaylistSongs(path)
                                } else if (type === "Song" ) {
                                    mpdConnector.appendSong(path)
                                }
                            }*/

                            function menu(path, type) {
                                if (type === "Song") {
                                    contextMenu.popup()
                                }
                            }

                            function onClicked(mouse) {
                                if (mouse.button === Qt.LeftButton) {
                                    root.itemClicked(type.toLowerCase(), path)
                                } else if (mouse.button === Qt.RightButton) {
                                    menu(path, type)
                                }
                            }
                            function onPressAndHold(mouse) {
                                if (mouse.source === Qt.MouseEventNotSynthesized) {
                                    menu(path, type)
                                }
                            }
                            function onDoubleClicked(mouse) {
                                root.itemClicked(type.toLowerCase(), path)
                            }
                        }
                        Menu {
                             id: contextMenu

                             MenuItem {
                                 text: qsTr("Add song")
                                 onTriggered: mpdConnector.appendSong(path)
                             }
                             MenuItem {
                                 text: qsTr("Add next")
                                 onTriggered: mpdConnector.moveSongAfterCurrent(songId)
                             }
                             MenuItem {
                                 text: qsTr("Add and replace")
                                 onTriggered: mpdConnector.moveSongAfterCurrent(songId)
                             }
                             MenuItem {
                                 text: qsTr("Add, replace and play")
                                 onTriggered: mpdConnector.moveSongAfterCurrent(songId)
                             }
                             MenuItem {
                                 text: qsTr("Add to playlist")
                                 onTriggered: mpdConnector.moveSongAfterCurrent(songId)
                             }
                             MenuItem {
                                 text: qsTr("Go to the artist")
                                 onTriggered: mpdConnector.moveSongAfterCurrent(songId)
                             }
                        }

                    }
                    Rectangle {
                        id: delegateItem
                        anchors.fill: parent
                        color: selected ? Material.accent : "transparent"

                        Text {
                            text: description
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                            anchors { fill: parent; margins: 3; leftMargin: 22 }
                        }
                    }
                }
            }
        }
    }


    Component.onCompleted: {
        loadModel()
    }
}
