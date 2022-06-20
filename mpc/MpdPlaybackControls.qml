import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    RowLayout {
        width: parent.width
        ToolButton {
            id: previous
            icon.name:"media-skip-backward"
            width: parent.width / parent.children.length
            onClicked: mpdConnector.previous()
        }
        ToolButton {
            id: play
            visible: !mpdConnector.playing
            icon.name: "media-playback-start"
            width: parent.width / parent.children.length
            onClicked: mpdConnector.play()
        }
        ToolButton {
            id: pause
            visible: mpdConnector.playing
            icon.name: "media-playback-pause"
            width: parent.width / parent.children.length
            onClicked: mpdConnector.pause()
        }
        ToolButton {
            id: stop
            icon.name: "media-playback-stop"
            width: parent.width / parent.children.length
            onClicked: mpdConnector.stop()
        }
        ToolButton {
            id: next
            icon.name:"media-skip-forward"
            width: parent.width / parent.children.length
            onClicked: mpdConnector.next()
        }
        ToolButton {
            id: repeat
            checked: mpdConnector.repeating
            icon.name: "media-playlist-repeat"
            width: parent.width / parent.children.length
            onClicked: mpdConnector.repeat(!checked)
        }
        ToolButton {
            id: shuffle
            checked: mpdConnector.shuffling
            icon.name: "media-playlist-shuffle"
            width: parent.width / parent.children.length
            onClicked: mpdConnector.random(!checked)
        }
    }
    background: Rectangle {
        radius: 20
        color: Material.primary
    }
}
