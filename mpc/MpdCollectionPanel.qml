import QtQuick
import QtQuick.Controls

Pane {
    id: root

    signal itemClicked(string type, string itemName)
    property var model;
    function loadModel() {}

    Column {
        width: parent.width
        height: parent.height

        ScrollView {
            width: parent.width
            height: parent.height
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            ListView {
                id: listView
                anchors.fill: parent
                model: root.model
                delegate: ItemDelegate {
                    width: listView.width
                    property bool selected: false /*model.isSelected(index)
                    Connections {
                        target: mpdConnector.collectionModel
                        function onSelectionChanged() {
                            selected = model.isSelected(index)
                        }
                    }*/

                    MouseArea {
                        id: itemDelegateMouseArea
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        Connections {
                            function menu(path, type) {
                                if (type === "Song") {
                                    contextMenu.popup()
                                }
                            }
                            function onClicked(mouse) {
                                if (mouse.button === Qt.LeftButton) {
                                    if (type.toLowerCase() === "artist") {
                                        root.itemClicked("album", path)
                                    } else {
                                        root.itemClicked("song", path)
                                    }
                                } else if (mouse.button === Qt.RightButton) {
                                    menu(path, type)
                                }
                            }
                            function onPressAndHold(mouse) {
                                if (mouse.source === Qt.MouseEventNotSynthesized) {
                                    menu(path, type)
                                }
                            }
                            function onDoubleClicked(mouse) {
                                if (type.toLowerCase() === "artist") {
                                    root.itemClicked("album", path)
                                } else {
                                    root.itemClicked("song", path)
                                }
                            }
                        }
                        Menu {
                             id: contextMenu

                             MenuItem {
                                 text: qsTr("Add song")
                                 onTriggered: mpdConnector.appendSong(path)
                             }
                             MenuItem {
                                 text: qsTr("Add next")
                                 onTriggered: mpdConnector.insertSong(path)
                             }
                             MenuItem {
                                 text: qsTr("Add and replace")
                                 onTriggered: {
                                     mpdConnector.clearQueue()
                                     mpdConnector.appendSong(path)
                                 }
                             }
                             MenuItem {
                                 text: qsTr("Add, replace and play")
                                 onTriggered: mpdConnector.moveSongAfterCurrent(songId)
                             }
                             MenuItem {
                                 text: qsTr("Add to playlist")
                                 onTriggered: mpdConnector.moveSongAfterCurrent(songId)
                             }
                             MenuItem {
                                 text: qsTr("Go to the artist")
                                 onTriggered: root.itemClicked("album", artist)
                             }
                             MenuItem {
                                 text: qsTr("Go to the album")
                                 onTriggered: root.itemClicked("song", album+'@'+artist)
                             }
                        }

                    }
                    Rectangle {
                        id: delegateItem
                        anchors.fill: parent
                        color: selected ? Material.accent : "transparent"

                        Text {
                            text: description
                            color: Material.foreground
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                            anchors { fill: parent; margins: 3; leftMargin: 22 }
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        loadModel()
    }
}
