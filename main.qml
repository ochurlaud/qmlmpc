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

import "mpc"
import "."
import "widgets"
import "mpc/mpd.js" as MPD

ApplicationWindow {
    id: appwindow
    visible: true
    property MpdSelectedSong selectedSong: selectedSong

    MpdSelectedSong {
        id: selectedSong
    }

    menuBar:  MenuBar {
        //color: "black"

        MenuBarItem {
            id: settingsButton
            text: "Settings"
            onClicked: settingsView.open()
        }
        MenuBarItem {
            text: "Now playing"
            onClicked: mainStack.clear()
        }
        MenuBarItem {
            id: playlistsButton
            text: "Playlists"
            onClicked: MPD.setActivePanel(MPD.playlistsPanel)
        }
        MenuBarItem {
            id: collectionButton
            text: "Collection"
            onClicked: MPD.setActivePanel(MPD.collectionPanel)
        }
        MenuBarItem {
            id: searchButton
            text: "Search"

            onClicked: MPD.setActivePanel(MPD.searchPanel)
        }
    }

    header:  Row {

        Button {
            id: previous
            icon.name:"media-skip-backward"
            onClicked: mpdConnector.previous()
        }
        Button {
            id: play
            visible: !mpdConnector.playing
            icon.name: "media-playback-start"
            onClicked: mpdConnector.play()
        }
        Button {
            id: pause
            visible: mpdConnector.playing
            icon.name: "media-playback-pause"
            onClicked: mpdConnector.pause()
        }
        Button {
            id: stop
            icon.name: "media-playback-stop"
            onClicked: mpdConnector.stop()
        }
        Button {
            id: next
            icon.name:"media-skip-forward"
            onClicked: mpdConnector.next()
        }
        Button {
            id: repeat
            checked: mpdConnector.repeating
            icon.name: "media-playlist-repeat"
            onClicked: mpdConnector.repeat(!checked)
        }
        Button {
            id: shuffle
            checked: mpdConnector.shuffling
            icon.name: "media-playlist-shuffle"
            onClicked: mpdConnector.random(!checked)
        }

    }

    StackView {
        id: mainStack
        anchors.fill: parent

        initialItem: MpdQueuePanel {
            width: parent.width
            height: parent.height
        }
        MpdPanel {
            id: mpdpanel
        }
    }
    Popup {
        id: settingsView

        Grid {
            id: settingsGrid
            anchors { centerIn: parent }
            columns: 2
            spacing: 3

            Label {
                width: 100
                height: 30
                text: "Connection settings to MPD server"
                font.pointSize: 16
            }
            Item{ width: 1; height: 1 }
            Label { width: 100; height: 30; text: "Host" }
            TextInput { id: mpdHost; width: 500; text: settings.value("mpd/host") ; }
            Label { width: 100; height: 30; text: "Port" }
            TextInput { id: mpdPort; width: 500; text: settings.value("mpd/port"); }
            Label { width: 100; height: 30; text: "Password" }
            TextInput { id: mpdPassword; width: 500; echoMode: TextInput.Password; text: settings.value("mpd/password") ;}
            Button {
                text: "Save changes"
                onClicked: saveSettings()
            }
            Button {
                text: "Discard changes"
                onClicked: discardSettings()
            }
        }
    }

    function resetSettings() {
        mpdHost.text = settings.value("mpd/host")
        mpdPort.text = settings.value("mpd/port")
        mpdPassword.text = settings.value("mpd/password")
    }

    function discardSettings() {
        settingsView.close()
    }

    function saveSettings() {
        settings.setValue("mpd/host", mpdHost.text)
        settings.setValue("mpd/port", mpdPort.text)
        settings.setValue("mpd/password", mpdPassword.text)
        mpdConnector.connection.reconnect(mpdHost.text, Number(mpdPort.text), mpdPassword.text)
        settingsView.close()
    }
}
