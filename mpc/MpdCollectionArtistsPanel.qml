import QtQuick 2.0
import org.qmlmpc

MpdCollectionPanel {
    function loadModel() {
        model.mpdConnector= mpdConnector
        model.queryContent()
    }

    model: MpdCollectionArtistsModel {
        id: model
        //mpdConnector: mpdConnector
    }
    //model: mpdConnector.collectionModel
}
