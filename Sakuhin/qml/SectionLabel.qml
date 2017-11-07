import QtQuick 2.7
import QtQuick.Controls 2.2

Label {
    property bool interactive: false
    property alias area: area

    height: 10
    color: "#bbb"
    text: "Section Label"
    anchors.left: parent.left
    anchors.leftMargin: 10
    anchors.rightMargin: 10
    font.pointSize: 7
    font.family: "Tahoma"
    anchors.right: parent.right

    Label {
        id: shaders_icon
        height: parent.height
        color: parent.color
        font: parent.font
        text: interactive ? '+' : '-'

        horizontalAlignment: Text.AlignRight
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 0
    }

    MouseArea {
        id: area
        anchors.fill: parent
    }
}
