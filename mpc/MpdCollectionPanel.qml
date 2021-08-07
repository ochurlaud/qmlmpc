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

import "../widgets"
import ".." // import Style.qml

import "mpd.js" as MPD

Pane {
    width: parent.width
    anchors.fill: parent

    property int numSelectedSongs: mpdConnector.collectionModel.numSelectedSongs
    property int numSelectedDirs: mpdConnector.collectionModel.numSelectedDirectories
    property bool hasSelection: numSelectedDirs+numSelectedSongs > 0
    property var parentList: []

    Column {
        width: parent.width
        ScrollView {
            width: parent.width
            height: 5*Style.rowHeight


            //color: "white"
            ListView {
                id: listView
                anchors.fill: parent
                model: mpdConnector.collectionModel
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
                        onClicked: {
                            mpdConnector.collectionModel.toggleSelection(index)
                        }
                        onDoubleClicked: {
                            parentList.push(path)
                            if (type === "Directory") {
                                mpdConnector.listDirectory(path)
                            } else if (type === "Artist") {
                                mpdConnector.listAlbums(path)
                            } else if (type === "Album") {
                                if (parentList.length > 1) {
                                    mpdConnector.listSongsByArtistAndAlbum(parentList[parentList.length - 2],
                                                                           path)
                                }
                            } else if (type === "Song" ) {
                                mpdConnector.appendSong(path)
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
        Row {
            MpdButton {
                width: 4*Style.first8ColumnsWidth
                text: "Add selected "+numSelectedSongs+" songs and "+numSelectedDirs+" directories"
                image: "images/append.png"
                enabled: hasSelection
                onClicked: mpdConnector.addSongs(mpdConnector.collectionModel.getSelectedPaths())
            }
            MpdButton {
                width: 2*Style.last4ColumnsWidth
                text: "Select all"
                onClicked: mpdConnector.collectionModel.selectAll()
            }
            MpdButton {
                width: 2*Style.last4ColumnsWidth
                text: "Select none"
                onClicked: mpdConnector.collectionModel.deselectAll()
            }
        }
    }

    Component {
        id: toolbarCombo
        ComboBox {
            model: ["Artists", "Albums", "Playlists", "Files", "Genres"]
        }
    }

    Component.onCompleted: {
        mpdConnector.listArtists()
        toolbarCentralItem.sourceComponent = toolbarCombo
    }
}
