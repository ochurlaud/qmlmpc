import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

Pane {
    id: settingsView

    property var mpdConnectionDetails: settings.mpdConnectionDetails("default")
    signal closeRequested()
    Column {
        anchors.fill: parent
        spacing: 20

        GridLayout {

            id: settingsGrid
            columns: 2
            //spacing: 3
            Label {
                Layout.fillHeight: true; Layout.columnSpan: 2
                text: "Connection settings to MPD server"
                font.pointSize: 16
            }
            Label { text: "Name" }
            TextField { id: mpdName; text: "default" ; }
            Label { text: "Host" }
            TextField { id: mpdHost; text: mpdConnectionDetails.host ; }
            Label { text: "Port" }
            TextField { id: mpdPort; text: mpdConnectionDetails.port ; inputMethodHints: Qt.ImhDigitsOnly ; validator: IntValidator {bottom: 0; top: 65535;} }
            Label { text: "Password" }
            TextField { id: mpdPassword; echoMode: TextInput.Password; text: mpdConnectionDetails.password ;}
        }
        Row {
            spacing: 20
            Button {
                text: "Save changes"
                onClicked: saveSettings()
            }
            Button {
                text: "Discard changes"
                onClicked: discardSettings()
            }
        }
    }
    function resetSettings() {
        mpdConnectionDetails = settings.mpdConnectionDetails("default")
        mpdHost.text = mpdConnectionDetails.host
        mpdPort.text = mpdConnectionDetails.port
        mpdPassword.text = mpdConnectionDetails.password
    }

    function discardSettings() {
        console.log("discard")
        resetSettings()
        settingsView.closeRequested()
    }

    function saveSettings() {
        settings.setValue("mpd/default/host", mpdHost.text)
        settings.setValue("mpd/default/port", mpdPort.text)
        settings.setValue("mpd/default/password", mpdPassword.text)
        mpdConnectionDetails.host = mpdHost.text
        mpdConnectionDetails.port = Number(mpdPort.text)
        mpdConnectionDetails.password = mpdPassword.text
        settings.setMpdConnectionDetails(mpdName.text, mpdConnectionDetails)

        mpdConnector.connection.reconnect(mpdName.text)
        settingsView.closeRequested();
    }
}
