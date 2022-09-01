import QtQuick
import QtQuick.Controls
import org.qmlmpc

Pane {
    id: root

    signal itemClicked(string type, string itemName)
    property var model: MpdCollectionAlbumsModel {
        id: model
        //mpdConnector: mpdConnector
    }
    property alias artist: model.artist;
    function loadModel() {
        model.mpdConnector = mpdConnector
        model.queryContent()
    }

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
                            function menu(albumName, type) {
                                contextMenu.popup()
                            }
                            function onClicked(mouse) {
                                if (mouse.button === Qt.LeftButton) {
                                    root.itemClicked("song", albumName + '@' + artist)
                                } else if (mouse.button === Qt.RightButton) {
                                    menu(albumName, type)
                                }
                            }
                            function onPressAndHold(mouse) {
                                if (mouse.source === Qt.MouseEventNotSynthesized) {
                                    menu(albumName, type)
                                }
                            }
                            function onDoubleClicked(mouse) {
                                root.itemClicked("song",  albumName + '@' + artist)
                            }
                        }
                        Menu {
                             id: contextMenu

                             MenuItem {
                                 text: qsTr("Add album")
                                 onTriggered: mpdConnector.appendAlbum(artist, albumName)
                             }
                             MenuItem {
                                 text: qsTr("Add next")
                                 onTriggered: mpdConnector.insertAlbum(artist, albumName)
                             }
                             MenuItem {
                                 text: qsTr("Add and replace")
                                 onTriggered: {
                                     mpdConnector.clearQueue()
                                     mpdConnector.appendAlbum(artist, albumName)
                                }
                             }
                             MenuItem {
                                 text: qsTr("Add, replace and play")
                                 onTriggered: {
                                     mpdConnector.clearQueue()
                                     mpdConnector.appendAlbum(artist, albumName, true)
                                }
                             }
                             MenuItem {
                                 text: qsTr("Add to playlist")
                                 onTriggered: {

                                 }
                             }
                             MenuItem {
                                 text: qsTr("Go to the artist")
                                 onTriggered: root.itemClicked("album", artist)
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
