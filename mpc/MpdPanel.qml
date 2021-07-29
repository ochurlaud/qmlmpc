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

import "mpd.js" as MPD
import "../widgets"
import ".." // import Style.qml

Rectangle {
    width: appwindow.width
    height: 8*Style.rowHeight
    //color: "black"


    Rectangle {
        anchors.fill: parent
        color: "black"
        Rectangle {
            color: "yellow"
            height: 100
            width:width
            visible: true //!mpdConnector.connection.connected
            Text {
                id: bugtext
                anchors.centerIn: parent

                text: "The connection to the server is lost.\n"+
                      "Click anywhere to try to restore the connection, or change your settings."
            }
            MouseArea {
                anchors.fill: parent
                onClicked: mpdConnector.connection.reconnect()
            }
        }


        Row {

            /*Rectangle {
                anchors.fill: playlistColumn
                color: "#88000000"
                visible: false //activePanel !== MPD.overviewPanel
                MouseArea {
                    anchors.fill: parent
                    onClicked: activePanel = MPD.overviewPanel
                }

            }*/


            /*MpdPlaylistsPanel {
                id: playlistsPanel
                visible: true //activePanel === MPD.playlistsPanel
            }*/
            /*MpdCollectionPanel {
                id: collectionPanel
                visible: true //activePanel === MPD.collectionPanel
            }*/
            MpdSearchPanel {
                id: searchPanel
                visible: false
            }
        }

    }

    // messagebox things:
    function confirm(text, obj) {
        messageLabel.text = text
        messagePrompt.visible = false
        messagebox.listeningObject = obj
        messagebox.visible = true
    }
    function prompt(text, obj) {
        messageLabel.text = text
        messagePrompt.visible = true
        messagePrompt.text = ""
        messagebox.listeningObject = obj
        messagebox.visible = true
        messagePrompt.forceActiveFocus()
    }

    Rectangle {
        id: messagebox

        anchors.fill: parent
        color: "black"
        visible: false

        property variant listeningObject

        function confirmed(text) {
            if (listeningObject.onConfirmed) {
                if (messagePrompt.visible) {
                    visible = false
                    listeningObject.onConfirmed(text)
                } else {
                    visible = false
                    listeningObject.onConfirmed()
                }
            }
        }

        function canceled() {
            visible = false
            if (listeningObject.onCanceled)
                listeningObject.onCanceled()
        }

        Column {
            anchors.centerIn: parent
            spacing: 5
            Text {
                id: messageLabel
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white";
                width: parent.parent.width*0.75
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }
            TextField {
                id: messagePrompt
                width: messageLabel.width
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 50
                MpdButton {
                    text: "OK"
                    color: "#080"
                    width: 3*Style.first8ColumnsWidth
                    onClicked: messagebox.confirmed(messagePrompt.text)
                }
                MpdButton {
                    text: "Cancel"
                    color: "#800"
                    width: 3*Style.first8ColumnsWidth
                    onClicked: messagebox.canceled()
                }
            }
        }
    }
}
