
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "mpc"
import "."

ApplicationWindow {
    id: appwindow
    visible: true

    property MpdSelectedSong selectedSong: selectedSong
    property string toolbarTitle: "QMLMPC - " + mpdConnector.connection.connectedServer

    MpdSelectedSong {
        id: selectedSong
    }
    menuBar: ToolBar {
        id: menuToolBar
        contentHeight: settingsButton.implicitHeight

        Item {
            anchors.centerIn: parent
            Loader {
                anchors.centerIn: parent
                id: toolbarCentralItem
            }
        }

        ToolButton {
            id: backButton
            anchors.left: parent.left
            icon.name: "draw-arrow-back"
            icon.color: mainStack.depth > 1 ? Material.foreground : Qt.alpha(Material.foreground, 0.3)
            onClicked: if (mainStack.depth > 1) { mainStack.pop() }
        }

        ToolButton {
            id: settingsButton
            anchors.left: backButton.right
            icon.name: "settings-configure"
            onClicked: mainStack.push(settingspanel)
        }
        ToolButton {
            id: searchButton
            anchors.right: parent.right
            icon.name: "search"
            onClicked: mainStack.push("qrc:///mpc/MpdSearchPanel.qml")
        }
    }

    footer: MpdViewSelector {
        width: parent.width
        buttonView: "queue"
        onButtonViewChanged: {
            console.log("buttonViewChanged:", buttonView)
            mainStack.clear()
            pushView(buttonView)
        }
    }

    StackView {
        id: mainStack
        anchors.fill: parent

        initialItem: "qrc:///mpc/MpdQueuePanel.qml"
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

    function pushView(viewType, name="", parentView="") {
        switch (viewType) {
        case "queue":
            mainStack.push("qrc:///mpc/MpdQueuePanel.qml")
            break;
        case "playlist":
            mainStack.push("qrc:///mpc/MpdCollectionPlaylistsPanel.qml")
            mainStack.currentItem.itemClicked.connect(function (type, name){
                pushView("songs", name=name, parentView="playlist")
            })
            break;
        case "artist":
            mainStack.push("qrc:///mpc/MpdCollectionArtistsPanel.qml")
            mainStack.currentItem.itemClicked.connect(function (type, name){
                pushView("album", name=name)
            })
            break;
        case "album":
            mainStack.push("qrc:///mpc/MpdCollectionAlbumsPanel.qml", {"artist": name})
            mainStack.currentItem.itemClicked.connect(function (type, name){
                pushView("songs", name=name, parentView="album")
            })
            break;
        case "songs":
            if (parentView === "album") {
                var namelist = name.split('@')
                var album = namelist[0]
                var artist = namelist[1]
                mainStack.push("qrc:///mpc/MpdCollectionSongsPanel.qml", {"artist": artist, "album": album})
                mainStack.currentItem.itemClicked.connect(function (type, name){
                  //  pushView("songs", name=name)
                })
            }
            else if (parentView === "playlist") {
                mainStack.push("qrc:///mpc/MpdCollectionSongsPanel.qml", {"playlist": name})
                mainStack.currentItem.itemClicked.connect(function (type, name){
                  //  pushView("songs", name=name)
                })
            }

            break;
        }
    }
}
