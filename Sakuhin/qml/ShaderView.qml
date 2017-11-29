import QtQuick 2.7
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

GridView {
    id: shader_view
    height: 70
    clip: true
    maximumFlickVelocity: 3000
    flickDeceleration: 1500
    boundsBehavior: Flickable.StopAtBounds
    snapMode: GridView.NoSnap
    cellHeight: 48
    cellWidth: 48

    signal shaderActivated()

    property int activeIndex
    property bool isViewPreviewed

    property alias contextArea: contextArea

    anchors.left: parent.left
    anchors.leftMargin: 10

    anchors.right: parent.right
    anchors.rightMargin: 2.5

    onCountChanged: currentIndex = count - 1
    onCurrentIndexChanged: shader_view.shaderActivated()

    delegate: Item {
        property bool isPressed: false

        width: shader_view.cellWidth
        height: shader_view.cellHeight

        opacity: ((GridView.isCurrentItem || isPressed || index == activeIndex) && isViewPreviewed) ? 1.0 : 0.5

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

        Label {
            visible: (index == currentIndex || index == activeIndex || isPressed)

            anchors.fill: parent
            anchors.topMargin: 0
            anchors.rightMargin: 5
            anchors.leftMargin: 0
            anchors.bottomMargin: 5

            color: "#fff"
            text: index == activeIndex ? "M" : "P"
            font.pointSize: shader_view.cellWidth * 0.45
            font.family: "Tahoma"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            style: Text.Outline
            styleColor: "#000"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                parent.GridView.view.currentIndex = index
                shader_view.shaderActivated()
            }
            onPressed: isPressed = true
            onReleased: isPressed = false

            // XXX: onExited is not emitted when the view is dragged
            // which results in the shader thinking it is still pressed
            // TODO: Replace mousearea logic with button logic
            onExited: isPressed = false
        }
    }

    MouseArea {
        anchors.fill: parent
        id: contextArea
        acceptedButtons: Qt.RightButton
    }
}
