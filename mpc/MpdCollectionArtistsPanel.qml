import QtQuick
import org.qmlmpc

MpdCollectionPanel {
    function loadModel() {
        model.mpdConnector= mpdConnector
        model.queryContent()
    }

    model: MpdCollectionArtistsModel {
        id: model
    }
}
