import QtQuick 2.7
import QtQuick.Controls 2.2

MenuItem {
    height: 30
    anchors.left: parent.left
    anchors.right: parent.right
    width: parent.width

    contentItem: Text {
        text: parent.text

        font.family: "Tahoma"
        font.pointSize: 8
        font.capitalization: Font.Capitalize

        color: "#ddd"
        horizontalAlignment: Text.AlignHLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        color: (parent.checked || parent.hovered) ? "#111117" : (parent.down ? "#111117" : "#27272f")
    }
}