import QtQuick 2.4
import QtQuick.Window 2.2
import QtMultimedia 5.6


Window {
    visible: true

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }

    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }



}
