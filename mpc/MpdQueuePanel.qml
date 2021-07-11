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
import QtQuick.Controls 2.15

import ".." // import Style.qml
import "../widgets"
import "mpd.js" as MPD


Rectangle {
    Column {

        id: queueColumn
        width: parent.width

        Item {
            id: progressBar
            width: parent.width
            height: Style.rowHeight/2
            Rectangle {
                anchors { fill: parent; margins: 1 }
                color: "#999"
                radius: 4
                clip: true
                Rectangle {
                    anchors { left: parent.left; top: parent.top; bottom: parent.bottom}
                    radius: Math.min(4, width/2)
                    color: "black"
                    width: mpdConnector.elapsedTime/mpdConnector.totalTime*parent.width
                    Rectangle {
                        x: 1
                        y: 1
                        width: Math.max(0, parent.width-2)
                        height: parent.height-2
                        color: "#444"
                        radius: Math.max(0, parent.radius-1)
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: mpdConnector.seek(mouseX*mpdConnector.totalTime/parent.width)
                }
            }
            Text {
                anchors.centerIn: parent
                text: MPD.formatSeconds(mpdConnector.elapsedTime)+" / "+MPD.formatSeconds(mpdConnector.totalTime)
                color: "white"
            }
        }

        Rectangle { 
            width: parent.width
            height: 5.5*Style.rowHeight
            TddListView {
                id: queueView
                anchors.fill: parent
                model: mpdConnector.queueModel
                property int savedScroll
                onCurrentIndexChanged: positionViewAtIndex(currentIndex, ListView.Visible)
                Connections {
                    target: queueView.model
                    onModelAboutToBeReset: queueView.savedScroll = queueView.contentY
                    onModelReset: {
                        queueView.contentY = queueView.savedScroll
                        var i = queueView.model.getSongIndex(appwindow.selectedSong.id)
                        if (i!==-1) queueView.currentIndex = i
                    }
                }
                delegate: Rectangle {
                    width: parent.width
                    height: Style.rowHeight/2
                    // color: steelblue if selected, else alternating row colors
                    color: songId===appwindow.appwindow.selectedSong.id ? "steelblue" : (index%2?"white":"#ddd")
                    Image {
                        id: img
                        source: "images/play-mini.png"
                        visible: songId===mpdConnector.currentSongId
                        anchors { left: parent.left; leftMargin: 3; verticalCenter: parent.verticalCenter }
                    }
                    Text {
                        text: description
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        anchors { fill: parent; margins: 3; }
                        anchors.leftMargin: img.visible?img.width+6:3
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            appwindow.selectedSong.id = songId
                            appwindow.selectedSong.title = title
                            appwindow.selectedSong.album = album
                            appwindow.selectedSong.artist = artist
                            appwindow.selectedSong.duration = duration
                            queueView.currentIndex = index
                        }
                        onDoubleClicked: mpdConnector.playSong(songId)
                    }
                }
            }
        }
        //Future FEATURE
        /*TextField {
            width: parent.width
            placeholderText: "Search in queue..."
            onEditingFinished:
        }*/

        Row {
            Button {
                text: "Scroll to current song"
                onClicked: queueView.positionViewAtIndex(mpdConnector.currentSongPos, ListView.Visible)
            }
            Button {
                text: "Scroll to selected song"
                onClicked: queueView.positionViewAtIndex(queueView.currentIndex, ListView.Visible)
            }
        }
    }
}

