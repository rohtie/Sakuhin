import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

BalancedGridView {
    id: shader_view

    property int activeIndex
    property bool isViewPreviewed

    property alias contextArea: contextArea

    heightMultiplier: 2.5

    signal shaderActivated()

    onCountChanged: currentIndex = count - 1
    onCurrentIndexChanged: shader_view.shaderActivated()

    delegate: Item {
        property bool isPressed: false

        width: shader_view.cellWidth
        height: shader_view.cellHeight

        opacity: ((GridView.isCurrentItem || isPressed || index == activeIndex) && isViewPreviewed) ? 1.0 : 0.5

        RoundImage {
            path: model.modelData.thumbnail
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
            font.family: "Arimo"
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
        id: contextArea

        anchors.fill: parent
        acceptedButtons: Qt.RightButton
    }
}
