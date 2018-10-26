import QtQuick 2.7
import QtGraphicalEffects 1.0

Image {
    property var path


    id: roundImage

    anchors.fill: parent
    anchors.topMargin: 0
    anchors.rightMargin: 5
    anchors.leftMargin: 0
    anchors.bottomMargin: 5

    asynchronous: true
    fillMode: Image.PreserveAspectCrop

    source: path || "qrc:assets/thumbnail.jpg"

    layer.enabled: true
    layer.effect: OpacityMask {
        maskSource: Item {
            width: roundImage.width
            height: roundImage.height

            Rectangle {
                anchors.fill: parent
                radius: 2
            }
        }
    }
}
