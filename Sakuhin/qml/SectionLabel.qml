import QtQuick 2.7
import QtQuick.Controls 2.2

Label {
    property bool interactive: false
    property alias area: area

    height: 15
    color: "#dddddd"
    text: "Section Label"
    anchors.left: parent.left
    anchors.leftMargin: 10
    anchors.rightMargin: 10
    font.pointSize: 8
    font.family: "Tahoma"
    anchors.right: parent.right

    Label {
        id: shaders_icon
        x: 0
        y: 0
        height: 15
        color: "#dddddd"
        text: interactive ? '+' : '-'
        horizontalAlignment: Text.AlignRight
        anchors.rightMargin: 0
        font.pointSize: 8
        anchors.left: parent.left
        anchors.right: parent.right
        font.family: "Tahoma"
        anchors.leftMargin: 0
    }

    MouseArea {
        id: area
        anchors.fill: parent
    }
}
