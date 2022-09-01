import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    RowLayout {
        width: parent.width
        ToolButton {
            id: previous
            icon.name:"media-skip-backward"
            Layout.fillWidth: true
            Layout.fillHeight: true
            onClicked: mpdConnector.previous()
        }
        ToolButton {
            id: play
            visible: !mpdConnector.playing
            icon.name: "media-playback-start"
            Layout.fillWidth: true
            Layout.fillHeight: true
            onClicked: mpdConnector.play()
        }
        ToolButton {
            id: pause
            visible: mpdConnector.playing
            icon.name: "media-playback-pause"
            Layout.fillWidth: true
            Layout.fillHeight: true
            onClicked: mpdConnector.pause()
        }
        ToolButton {
            id: stop
            icon.name: "media-playback-stop"
            Layout.fillWidth: true
            Layout.fillHeight: true
            onClicked: mpdConnector.stop()
        }
        ToolButton {
            id: next
            icon.name:"media-skip-forward"
            Layout.fillWidth: true
            Layout.fillHeight: true
            onClicked: mpdConnector.next()
        }
        ToolButton {
            id: repeat
            checked: mpdConnector.repeating
            icon.name: "media-playlist-repeat"
            Layout.fillWidth: true
            Layout.fillHeight: true
            onClicked: mpdConnector.repeat(!checked)
        }
        ToolButton {
            id: shuffle
            checked: mpdConnector.shuffling
            icon.name: "media-playlist-shuffle"
            Layout.fillWidth: true
            Layout.fillHeight: true
            onClicked: mpdConnector.random(!checked)
        }
        ToolButton {
            id: consume
            checked: mpdConnector.consuming
            icon.name: "edit-delete-symbolic"
            Layout.fillWidth: true
            Layout.fillHeight: true
            onClicked: mpdConnector.consume(!checked)
        }
    }
    background: Rectangle {
        radius: 20
        color: Material.primary
    }
}
