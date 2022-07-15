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
import QtQuick.Layouts

Pane {
    Column {

        id: queueColumn
        anchors.fill: parent

        Item {
            id: progressBar
            width: parent.width
            height: 30
            Rectangle {
                anchors { fill: parent; margins: 1 }
                color: Material.primary
                radius: 4
                clip: true
                Rectangle {
                    anchors { left: parent.left; top: parent.top; bottom: parent.bottom}
                    radius: Math.min(4, width/2)
                    width: mpdConnector.elapsedTime/mpdConnector.totalTime*parent.width
                    color: Material.accent
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: mpdConnector.seek(mouseX*mpdConnector.totalTime/parent.width)
                }
            }
            Text {
                anchors.centerIn: parent
                text: "%1 / %2".arg(formatSeconds(mpdConnector.elapsedTime))
                               .arg(formatSeconds(mpdConnector.totalTime))
                color: Material.foreground
            }
        }

        PlaylistView {
            width: parent.width
            height: parent.height - progressBar.height - buttonrow.height - playbackControls.height
            Component.onCompleted: console.log(height)
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

        MpdPlaybackControls {
            id: playbackControls
            width: 0.8* parent.width
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
    function formatSeconds(inputSeconds) {
        var seconds = inputSeconds % 60
        var secondsInMinutes = (inputSeconds - seconds) / 60
        var minutes = secondsInMinutes % 60
        var hours = (secondsInMinutes - minutes) / 60

        var timeString = ""
        if (hours > 0) {
            timeString += "%1:%2:".arg(hours).arg(String(minutes).padStart(2, '0'))
        } else {
            timeString += "%1:".arg(minutes)
        }
        timeString += String(seconds).padStart(2, '0')
        return timeString
    }
}
