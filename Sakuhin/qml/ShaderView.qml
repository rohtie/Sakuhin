import QtQuick 2.7
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

GridView {
    id: shader_view
    height: 60
    clip: true
    maximumFlickVelocity: 3000
    flickDeceleration: 1500
    boundsBehavior: Flickable.StopAtBounds
    snapMode: GridView.NoSnap
    cellHeight: 48
    cellWidth: 48

    anchors.left: parent.left
    anchors.leftMargin: 10

    anchors.right: parent.right
    anchors.rightMargin: 2.5

    onCountChanged: currentIndex = count - 1

    delegate: Item {
        width: shader_view.cellWidth
        height: shader_view.cellHeight

        opacity: GridView.isCurrentItem ? 1.0 : 0.5

        Image {
            id: shaderImage
            anchors.fill: parent
            anchors.topMargin: 0
            anchors.rightMargin: 5
            anchors.leftMargin: 0
            anchors.bottomMargin: 5

            asynchronous: true
            fillMode: Image.PreserveAspectCrop
            source: model.modelData.thumbnail

            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Item {
                    width: shaderImage.width
                    height: shaderImage.height

                    Rectangle {
                        anchors.fill: parent
                        radius: 2
                    }
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: parent.GridView.view.currentIndex = index
        }
    }
}
