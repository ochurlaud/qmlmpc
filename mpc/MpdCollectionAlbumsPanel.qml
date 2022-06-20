import QtQuick 2.0

MpdCollectionPanel {

    property string artist: ""

    function loadModel() {
        mpdConnector.listAlbums(artist)
    }

    model: mpdConnector.collectionModel
}
