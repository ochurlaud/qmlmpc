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
        id: menuToolBar
        contentHeight: toolButton1.implicitHeight
        Item {
            anchors.centerIn: parent
            Loader {
                anchors.centerIn: parent
                id: toolbarCentralItem
            }
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
                    mainStack.pop()
                    mainStack.push(collectionPanelComponent)
                    drawer.close()
                }
            }
            ItemDelegate {
                width: parent.width
                text: qsTr("Queue")
                onClicked: {
                    mainStack.pop()
                    drawer.close()
                }
            }
            ItemDelegate {
                width: parent.width
                text: qsTr("Settings")
                onClicked: {
                    mainStack.pop()
                    mainStack.push(settingsPanel)
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

        initialItem: queuePanelComponent
    }
    Component {
        id: queuePanelComponent
        MpdQueuePanel {}
    }

    Component {
        id: collectionPanelComponent
        MpdCollectionPanel {}
    }

    SettingsPanel {
        id: settingspanel
        visible: false
    }

    Connections {
        target: settingspanel
        function onCloseRequested() {
            mainStack.pop()
        }
    }


    Component {
        id: mainToolbarTitle
        Label {
            text: appwindow.toolbarTitle
        }
    }

    Component.onCompleted: {
        toolbarCentralItem.sourceComponent = mainToolbarTitle
    }


}
