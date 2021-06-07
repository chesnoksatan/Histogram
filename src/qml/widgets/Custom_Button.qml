import QtQuick 2.10

Rectangle {
    id: button

    implicitWidth: 100
    height: 25

    property alias text: txt.text
    property alias textColor: txt.color
    property alias textPointSize: txt.font.pointSize

    signal clicked()

    property color pressedColor: "#C0C0C0"
    property color notPressedColor: "#DCDCDC"
    property color hoverColor: 'powderblue'
    property color checkedColor: "#DCDCDC"

    property bool checked: false

    textColor: "Black"

    border.color: "DarkGray"
    color: mouseArea.hover ? (mouseArea.pressed ? pressedColor : hoverColor) :
                             (checked ? checkedColor : notPressedColor)

    radius: 2

    Text {
        id: txt

        anchors.centerIn: parent

        font.pointSize: 10

        opacity: enabled ? 1.0 : 0.6

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        id: mouseArea

        property bool hover: false

        anchors.fill: parent

        hoverEnabled: true

        onHoveredChanged: { hover = !hover }

        onClicked: button.clicked()
    }
}
