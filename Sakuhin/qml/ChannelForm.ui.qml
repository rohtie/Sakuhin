import QtQuick 2.7
import QtQuick.Templates 2.2
import QtGraphicalEffects 1.0

Button {
    id: channel

    width: 43
    height: 43

    property alias channel: channel
    property alias channelImage: channelImage

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
