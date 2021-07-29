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
import QtQuick.Layouts 1.12

import "mpc"
import "."
import "widgets"
import "mpc/mpd.js" as MPD

ApplicationWindow {
    id: appwindow
    visible: true

    property MpdSelectedSong selectedSong: selectedSong
    property string toolbarTitle: "QMLMPC"

    MpdSelectedSong {
        id: selectedSong
    }
    menuBar: ToolBar {
        contentHeight: toolButton1.implicitHeight
        Label {
            text: appwindow.toolbarTitle
            anchors.centerIn: parent
        }
        ToolButton {
            id: toolButton1
            text: "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: drawer.open()
        }
        ToolButton {
            id: search
            anchors.right: parent.right
            text: "\uD83D\uDD0D"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: mainStack.push(searchpanel)
        }
    }

    Drawer {
        id: drawer
        height: appwindow.height
        width: appwindow.width * 0.66
        Column {
            anchors.fill: parent
            ItemDelegate {
                width: parent.width
                text: qsTr("Browse collection")
                onClicked: {
                    mainStack.push(collectionpanel)
                    drawer.close()
                }
            }
            ItemDelegate {
                width: parent.width
                text: qsTr("Queue")
                onClicked: {
                    mainStack.pop(queuePanel)
                    drawer.close()
                }
            }
            ItemDelegate {
                width: parent.width
                text: qsTr("Settings")
                onClicked: {
                    mainStack.push(settingspanel)
                    drawer.close()
                }
            }
        }
    }

    footer:  Row {

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

        initialItem: queuePanel
    }
    MpdQueuePanel {
        id: queuePanel
        visible: false
    }

    MpdCollectionPanel {
        id: collectionpanel
        visible: false
    }

    SettingsPanel {
        id: settingspanel
        visible: false

    }
    Connections {
        target: settingspanel
        function onCloseRequested() {
            console.debug("ooo")
            mainStack.pop()
        }
    }

}
