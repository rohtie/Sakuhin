import QtQuick 2.7
import QtQuick.Templates 2.2
import QtGraphicalEffects 1.0

Button {
    id: channel

    width: 52
    height: 52
    property alias channel: channel

    background: Rectangle {
        anchors.fill: parent
        color: "#69697b"
        radius: 2

        Image {
            id: channelImage

            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop

            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Item {
                    width: channelImage.width
                    height: channelImage.height

                    Rectangle {
                        anchors.fill: parent
                        radius: 2
                    }
                }
            }
        }
    }
}
