import QtQuick 2.0
import org.qmlmpc

MpdCollectionPanel {
    function loadModel() {
        toto.mpdConnector= mpdConnector
        console.log("otot")
        model.listArtists()
    }

    model: MpdCollectionModel {
        id: toto
        //mpdConnector: mpdConnector
    }
    //model: mpdConnector.collectionModel
}
