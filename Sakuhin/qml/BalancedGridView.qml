import QtQuick 2.7
import QtQuick.Layouts 1.3

GridView {
    property var cellsPerRow: 5
    property var cellSize: parent.width / cellsPerRow
    property var heightMultiplier: 1.0

    Layout.fillWidth: true
    Layout.minimumHeight: cellSize * heightMultiplier

    cellWidth: cellSize
    cellHeight: cellSize

    clip: true

    maximumFlickVelocity: 3000
    flickDeceleration: 1500
    boundsBehavior: Flickable.StopAtBounds
    snapMode: GridView.NoSnap

}
