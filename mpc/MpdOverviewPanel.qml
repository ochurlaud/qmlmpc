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

import QtQuick 2.1

import ".." // import Style.qml
import "mpd.js" as MPD

Rectangle {
    width: 4*Style.first8ColumnsWidth+4*Style.last4ColumnsWidth
    height: 7*Style.rowHeight
    color: "black"

    Connections {
        target: mpdConnector.queueModel
        function onModelReset() {
            if (appwindow.selectedSong.id != -1 && !mpdConnector.queueModel.containsId(appwindow.selectedSong.id)) {
                appwindow.selectedSong.id = -1
            }
        }
    }

    Column {
        MpdText {
            width: 4*Style.first8ColumnsWidth
            height: Style.rowHeight/2
            text: "Selected song"
        }
        Item {
            width: parent.width
            height: 2*Style.rowHeight
            Column {
                id: songInfoColumn
                y: 5
                x: 10
                Text{
                    height: titleContent.height
                    text: "Title:"
                    color: "white"
                }
                Text {
                    height: artistContent.height
                    text: "Artist:"
                    color: "white"
                }
                Text {
                    height: albumContent.height
                    text: "Album:"
                    color: "white"
                }
                Text {
                    height: lengthContent.height
                    text: "Duration:"
                    color: "white"
                }
            }
            Column {
                anchors { left: songInfoColumn.right; leftMargin: 10; right: parent.right; top: songInfoColumn.top }
                Text {
                    id: titleContent
                    width: parent.width
                    text: appwindow.selectedSong.songSelected?appwindow.selectedSong.title:"-"
                    wrapMode: Text.WordWrap
                    color: "white"
                }
                Text {
                    id: artistContent
                    width: parent.width
                    text: appwindow.selectedSong.songSelected?appwindow.selectedSong.artist:"-"
                    wrapMode: Text.WordWrap
                    color: "white"
                }
                Text {
                    id: albumContent
                    width: parent.width
                    text: appwindow.selectedSong.songSelected?appwindow.selectedSong.album:"-"
                    wrapMode: Text.WordWrap
                    color: "white"
                }
                Text {
                    id: lengthContent
                    width: parent.width
                    text: appwindow.selectedSong.songSelected?MPD.formatSeconds(appwindow.selectedSong.duration):"-:--"
                    wrapMode: Text.WordWrap
                    color: "white"
                }
            }
        }
        Row {
            MpdButton {
                width: 2*Style.first8ColumnsWidth
                text: "Play now"
                image: "images/play-small.png"
                enabled: appwindow.selectedSong.songSelected
                onClicked: mpdConnector.playSong(appwindow.selectedSong.id)
            }
            MpdButton {
                width: 2*Style.first8ColumnsWidth
                text: "Remove from playlist"
                image: "images/remove.png"
                enabled: appwindow.selectedSong.songSelected
                onClicked: mpdConnector.removeSong(appwindow.selectedSong.id)
            }
        }
        MpdText{
            width: parent.width
            height: Style.rowHeight/2
            text: "Current playlist"
        }
        Item {
            width: parent.width
            height: 2*Style.rowHeight
            Text {
                width: parent.width
                y: 5
                x: 10
                text: "Number of songs: "+mpdConnector.numSongs
                color: "white"
            }
        }
        Row {
            MpdButton {
                id: saveButton
                width: 2*Style.first8ColumnsWidth
                text: "Save playlist"
                image: "images/save.png"
                onClicked: onConfirmed("")
                function onConfirmed(text) { // called when prompt --v is confirmed
                    if (text === "")
                        return prompt("Enter a name for this playlist:", saveButton)
                    mpdConnector.savePlaylist(text)
                }
            }
            MpdButton {
                width: 2*Style.first8ColumnsWidth
                text: "Clear playlist"
                image: "images/remove.png"
                onClicked: mpdConnector.clearQueue()
            }
        }
    }
    Column {
        x: 4*Style.first8ColumnsWidth
        MpdText {
            width: parent.width
            height: Style.rowHeight/2
            text: "Move selected song"
        }
        Row {
            MpdButton {
                width: 2*Style.last4ColumnsWidth
                text: "Up"
                image: "images/arrow-up.png"
                enabled: appwindow.selectedSong.songSelected
                onClicked: mpdConnector.moveSongUp(appwindow.selectedSong.id)
            }
            MpdButton {
                width: 2*Style.last4ColumnsWidth
                text: "Down"
                image: "images/arrow-down.png"
                enabled: appwindow.selectedSong.songSelected
                onClicked: mpdConnector.moveSongDown(appwindow.selectedSong.id)
            }
        }
        Row {
            MpdButton {
                width: 2*Style.last4ColumnsWidth
                text: "To top"
                image: "images/arrow-top.png"
                enabled: appwindow.selectedSong.songSelected
                onClicked: mpdConnector.moveSongFirst(appwindow.selectedSong.id)
            }
            MpdButton {
                width: 2*Style.last4ColumnsWidth
                text: "To bottom"
                image: "images/arrow-bottom.png"
                enabled: appwindow.selectedSong.songSelected
                onClicked: mpdConnector.moveSongLast(appwindow.selectedSong.id)
            }
        }
        MpdButton {
            width: 2*Style.last4ColumnsWidth
            text: "After currently playing song"
            enabled: appwindow.selectedSong.songSelected && mpdConnector.currentSongId!==-1
            onClicked: mpdConnector.moveSongAfterCurrent(appwindow.selectedSong.id)
        }
    }
}
