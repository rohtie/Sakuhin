import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Templates 2.2

Slider {
    id: control

    width: 20
    height: parent.height

    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

    wheelEnabled: true
    orientation: Qt.Vertical

    background: Rectangle {
        width: 7
        height: control.height
        x: control.leftPadding
        y: control.topPadding + control.height / 2 - height / 2
        anchors.horizontalCenter: parent.horizontalCenter

        color: "#69697b"
        radius: 2

        Rectangle {
            anchors.bottom: parent.bottom

            width: parent.width
            height: (1. - control.visualPosition) * parent.height
            color: "#21be2b"
            radius: 2
        }
    }
}
