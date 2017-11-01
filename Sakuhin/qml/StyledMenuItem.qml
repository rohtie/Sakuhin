import QtQuick 2.7
import QtQuick.Controls 2.2

MenuItem {
    height: 40

    contentItem: Text {
        text: parent.text
        font: parent.font
        color: "#ddd"
        horizontalAlignment: Text.AlignHLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        color: (parent.checked || parent.hovered) ? "#111117" : (parent.down ? "#111117" : "#27272f")
    }
}