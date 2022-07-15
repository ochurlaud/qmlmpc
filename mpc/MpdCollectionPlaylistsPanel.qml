import QtQuick 2.0
import org.qmlmpc

MpdCollectionPanel {
    function loadModel() {
        model.mpdConnector= mpdConnector
        model.queryContent()
    }

    model: MpdCollectionPlaylistsModel {
        id: model
        //mpdConnector: mpdConnector
    }
    //model: mpdConnector.collectionModel
}