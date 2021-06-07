import QtQml 2.3
import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import Qt.labs.settings 1.0

import "widgets"

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: "Histogram"

    // Для сохранения позиции и размеров окна
    Settings {
        id: settings
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
    }

    RowLayout {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 5
        }

        Custom_TextField {
            id: file_name
            Layout.fillWidth: true
        }

        Custom_Button {
            text: "Выберите файл"

            onClicked: fileDialog.open()
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        nameFilters: [ "Text files (*.txt)" ]
        onAccepted: {
            file_name.text = fileDialog.fileUrl.toString()
            Histogram.setFile(fileDialog.fileUrl.toString())
        }
    }
}
