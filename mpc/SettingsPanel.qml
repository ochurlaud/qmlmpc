import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

Pane {
    id: settingsView
    visible: false
    anchors.fill: parent
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
            Label { text: "Host" }
            TextField { id: mpdHost; text: settings.value("mpd/host") ; }
            Label { text: "Port" }
            TextField { id: mpdPort; text: settings.value("mpd/port"); }
            Label { text: "Password" }
            TextField { id: mpdPassword; echoMode: TextInput.Password; text: settings.value("mpd/password") ;}
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
        mpdHost.text = settings.value("mpd/host")
        mpdPort.text = settings.value("mpd/port")
        mpdPassword.text = settings.value("mpd/password")
    }

    function discardSettings() {
        console.log("discard")
        settingsView.closeRequested();
    }

    function saveSettings() {
        settings.setValue("mpd/host", mpdHost.text)
        settings.setValue("mpd/port", mpdPort.text)
        settings.setValue("mpd/password", mpdPassword.text)
        mpdConnector.connection.reconnect(mpdHost.text, Number(mpdPort.text), mpdPassword.text)
        settingsView.closeRequested();
    }
}
