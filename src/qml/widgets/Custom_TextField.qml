import QtQuick 2.10
import QtQuick.Controls 2.0

TextField {
    id: control

    implicitWidth: 100
    implicitHeight: 25
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter

    font.pointSize: 10
    selectByMouse: true

    bottomPadding: 2
    topPadding: 2
    leftPadding: 2
    rightPadding: 2

    property color defaultColor: "DarkGray"
    property color failureColor: "#ff7065"

    readonly property string errorState: "error"
    readonly property string normalState: "normal"

    state: normalState
    states: [
        State {
            name: errorState
            PropertyChanges {
                target: bcg
                border.color: control.failureColor
                border.width: 2
            }
        }
    ]

    background: Rectangle {
        id: bcg
        color: "#ffffff"
        border.color: "DarkGray"
        border.width: 1

        anchors.fill: parent

        radius: 2

        MouseArea {
            anchors.fill: parent

            hoverEnabled: true

            onEntered: {
                if ( control.state === control.errorState )
                    bcg.border.color = "#ffb1ab"
                else
                    bcg.border.color = "powderblue"
            }

            onExited: {
                if ( control.state === control.errorState )
                    bcg.border.color = control.failureColor
                else
                    bcg.border.color = control.defaultColor
            }

            onClicked: mouse.accepted = false;
            onPressed: mouse.accepted = false;
            onReleased: mouse.accepted = false;
            onDoubleClicked: mouse.accepted = false;
            onPositionChanged: mouse.accepted = false;
            onPressAndHold: mouse.accepted = false;
        }
    }
}
