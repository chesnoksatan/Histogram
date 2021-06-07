import QtQuick 2.10
import QtQuick.Window 2.10
import Qt.labs.settings 1.0

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: "Histogram"

    Settings {
        id: settings
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
    }
}
