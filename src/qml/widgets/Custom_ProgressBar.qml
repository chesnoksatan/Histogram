import QtQuick 2.10
import QtQuick.Controls 2.2

ProgressBar {
    id: control
    padding: 5

    implicitWidth: 200
    implicitHeight: 25
    to: 100

    background: Rectangle {
        anchors.fill: parent
        color: "#dcdcdc"
        border.color: "DarkGray"
        radius: 2
    }

    Label {z: 999; text: control.value + '%'; anchors.horizontalCenter: parent.horizontalCenter;anchors.verticalCenter: parent.verticalCenter}

    contentItem: Item {
        implicitWidth: 200
        implicitHeight: 25

        Rectangle {
            width: control.visualPosition * parent.width
            height: parent.height
            radius: 2
            color: "#00bfff"
        }
    }
}
