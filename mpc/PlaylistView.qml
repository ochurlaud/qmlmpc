import QtQuick 2.1
import QtQuick.Controls 2.15

ScrollView {
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ListView {
        id: queueView
        anchors.fill: parent
        model: mpdConnector.queueModel
        property int savedScroll
        onCurrentIndexChanged: positionViewAtIndex(currentIndex, ListView.Visible)
        Connections {
            target: queueView.model
            function onModelAboutToBeReset() {
                queueView.savedScroll = queueView.contentY
            }
            function onModelReset() {
                queueView.contentY = queueView.savedScroll
                var i = queueView.model.getSongIndex(appwindow.selectedSong.id)
                if (i!==-1) queueView.currentIndex = i
            }
        }
        delegate: ItemDelegate {
            width: queueView.width
            Text {
                id: playIcon
                text: "\u25B6"
                visible: songId===mpdConnector.currentSongId
                anchors { left: parent.left; leftMargin: 3; verticalCenter: parent.verticalCenter }
            }
            Text {
                text: description
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                anchors { fill: parent; margins: 3; }
                anchors.leftMargin: playIcon.visible?playIcon.width+6:3
            }
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                Connections {
                    function onDoubleClicked(mouse) {
                        mpdConnector.playSong(songId)
                    }
                    function onClicked(mouse) {
                        queueView.currentIndex = index
                        if (mouse.button === Qt.RightButton) {
                            contextMenu.popup()
                        }
                    }
                    function onPressAndHold(mouse) {
                        queueView.currentIndex = index
                        if (mouse.source === Qt.MouseEventNotSynthesized) {
                            contextMenu.popup()
                        }
                    }
                }
                Menu {
                    id: contextMenu

                    MenuItem {
                        text: qsTr("Play next")
                        onTriggered: mpdConnector.moveSongAfterCurrent(songId)
                    }
                    MenuItem {
                        text: qsTr("Move first")
                        onTriggered: mpdConnector.moveSongFirst(songId)
                    }
                    MenuItem {
                        text: qsTr("Move last")
                        onTriggered: mpdConnector.moveSongLast(songId)
                    }
                    MenuItem {
                        text: qsTr("Remove from playlist")
                        onTriggered: mpdConnector.removeSong(songId)
                    }
                }
            }
        }
    }
}
