import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    property string buttonView: ""
    RowLayout {
        width: parent.width
        ToolButton {
            id: artistButton
            icon.name:"view-media-artist"
            width: parent.width / parent.children.length
            checked: buttonView == "artist"
            onClicked:
                buttonView = "artist"
        }
        ToolButton {
            id: albumButton
            icon.name: "view-media-album-cover"
            width: parent.width / parent.children.length
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
            width: parent.width / parent.children.length
            checked: buttonView == "playlist"
            onClicked:
                buttonView = "playlist"
        }
        ToolButton {
            id: queueButton
            icon.name:"media-playlist-play"
            width: parent.width / parent.children.length
            checked: buttonView == "queue"
            onClicked:
                buttonView = "queue"
        }
    }
}
