import QtQuick
import org.qmlmpc

MpdCollectionPanel {
    property alias artist: model.artist
    property alias album: model.album
    property alias playlist: model.playlist

    function loadModel() {
        model.mpdConnector = mpdConnector
        model.queryContent()
    }

    model: MpdCollectionSongsModel {
        id: model
    }
}
