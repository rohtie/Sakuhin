import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Label {
    property bool interactive: false
    property alias area: area

    height: 10
    color: "#bbb"
    text: "Section Label"

    font.pointSize: 7
    font.family: "Arimo"

    Layout.fillWidth: true

    Label {
        id: shaders_icon
        height: parent.height
        color: parent.color
        font: parent.font
        text: interactive ? '+' : '-'

        horizontalAlignment: Text.AlignRight

        anchors.right: parent.right
    }

    MouseArea {
        id: area
        anchors.fill: parent
    }
}
