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


Pane {
    Column {

        id: queueColumn
        width: parent.width
        anchors.fill: parent

        Item {
            id: progressBar
            width: parent.width
            height: Style.rowHeight/2
            Rectangle {
                anchors { fill: parent; margins: 1 }
                color: Material.primary
                radius: 4
                clip: true
                Rectangle {
                    anchors { left: parent.left; top: parent.top; bottom: parent.bottom}
                    radius: Math.min(4, width/2)
           //         color: "black"
                    width: mpdConnector.elapsedTime/mpdConnector.totalTime*parent.width
                    Rectangle {
                        x: 1
                        y: 1
                        width: Math.max(0, parent.width-2)
                        height: parent.height-2
                        color: Material.accent
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
                color: Material.foreground
            }
        }

        PlaylistView {
            width: parent.width
            height: parent.height - progressBar.height - buttonrow.height
        }

        //Future FEATURE
        /*TextField {
            width: parent.width
            placeholderText: "Search in queue..."
            onEditingFinished:
        }*/

        Row {
            id: buttonrow
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


