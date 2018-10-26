import QtQuick 2.7
import QtQuick.Controls 2.2

TabButton {
    anchors.top: parent.top

    height: parent.height

    contentItem: Text {
        text: parent.text
        font.family: "Arimo"
        font.pointSize: 9
        opacity: 1.0
        color: "#ddd"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        color: parent.checked ? "#111117" : (parent.down ? "#111117" : "#27272f")
    }
}
