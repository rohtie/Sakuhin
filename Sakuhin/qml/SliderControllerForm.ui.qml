import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Templates 2.2

Slider {
    id: control
    width: 10
    height: 133
    Layout.fillWidth: true
    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
    Layout.fillHeight: true
    wheelEnabled: true
    orientation: Qt.Vertical

    background: Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        x: control.leftPadding
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: 10
        implicitHeight: 100
        width: implicitWidth
        height: control.availableHeight
        radius: 2
        color: "#69697b"

        Rectangle {
            anchors.bottom: parent.bottom

            width: parent.width
            height: (1. - control.visualPosition) * parent.height
            color: "#21be2b"
            radius: 2
        }
    }
}
