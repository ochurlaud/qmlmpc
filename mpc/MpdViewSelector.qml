import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    property string buttonView: ""
    RowLayout {
        anchors.fill : parent

        ToolButton {
            id: artistButton
            icon.name:"view-media-artist"
            Layout.fillWidth: true
            Layout.fillHeight: true
            checked: buttonView == "artist"
            onClicked:
                buttonView = "artist"
        }
        ToolButton {
            id: albumButton
            icon.name: "view-media-album-cover"
            Layout.fillWidth: true
            Layout.fillHeight: true
            checked: buttonView == "album"
            onClicked:
                buttonView = "album"
        }
/*
        ToolButton {
            id: genreButton
            icon.name: "view-media-genre"
            width: parent.width / parent.children.length
            checked: buttonView == "genre"
            onClicked:
                buttonView = "genre"
        }
*/
        ToolButton {
            id: playlistButton
            icon.name: "view-media-playlist"
            Layout.fillWidth: true
            Layout.fillHeight: true
            checked: buttonView == "playlist"
            onClicked:
                buttonView = "playlist"
        }
        ToolButton {
            id: queueButton
            icon.name:"media-playlist-play"
            Layout.fillWidth: true
            Layout.fillHeight: true
            checked: buttonView == "queue"
            onClicked:
                buttonView = "queue"
        }
    }
}
