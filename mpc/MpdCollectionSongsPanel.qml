import QtQuick 2.0

MpdCollectionPanel {
    property string artist: ""
    property string album: ""

    Component.onCompleted: {
        mpdConnector.listSongsByArtistAndAlbum(artist, album)
    }
}
