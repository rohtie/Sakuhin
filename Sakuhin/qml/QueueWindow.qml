import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    id: queue

    flags: Qt.FramelessWindowHint
    visible: true
    width: 1920
    height: 100
    x: 0
    y: 540
    color: "#111117"
    title: "Queue"

    property double zoomProportion: 1.0 / 500.0

    property int dragHandleWidth: 20

    property int sceneWidth: 50000

    function adjustZoom(wheel){
        var previousZoomProportion = queue.zoomProportion

        var newZoomProportion = previousZoomProportion + (wheel.angleDelta.y / 120) * 0.001

        if (newZoomProportion < 1.0 / 750.0) {
            newZoomProportion = previousZoomProportion
        }

        queue.zoomProportion = newZoomProportion
    }

    MouseArea {
        width: queue.width
        height: queue.height

        onWheel: {
            adjustZoom(wheel)
        }
    }

    Flickable {
        id: flickable

        y: 30

        width: queue.width
        height: queue.height - 30

        contentWidth: queuemanager.queue.count * queue.sceneWidth

        focus: true

        Keys.onPressed: {
            if (event.key == Qt.Key_N) {
                queuemanager.newScene()
            }
            else if (event.key == Qt.Key_PageUp) {
                queuemanager.previousScene();
            }
            else if (event.key == Qt.Key_PageDown) {
                queuemanager.nextScene();
            }
            else if (event.key == Qt.Key_F2) {
                if (sceneContainer.isRenaming) {
                    sceneContainer.isRenaming = false
                    flickable.forceActiveFocus()
                }
                else {
                    sceneContainer.isRenaming = true
                }

            }
        }

        Item {
            Row {
                id: sceneContainer

                property bool isRenaming: false

                height: flickable.height

                Repeater {
                    model: queuemanager.queue

                    Rectangle {
                        id: scene

                        width: queue.sceneWidth * queue.zoomProportion
                        height: parent.height

                        color: (index == queuemanager.sceneIndex) ? "#21be2b" : "#69697b"

                        border.width: 1
                        border.color: "#111117"

                        radius: 2

                        Text {
                            height: parent.height
                            width: parent.width
                            text: model.modelData.name
                            leftPadding: 10
                            color: "#111117"
                            verticalAlignment: Text.AlignVCenter
                            clip: true
                        }

                        TextField {
                            visible: sceneContainer.isRenaming
                            height: parent.height
                            width: parent.width
                            text: model.modelData.name
                            leftPadding: 10
                            color: "#111117"
                            verticalAlignment: Text.AlignVCenter
                            onTextChanged: model.modelData.name = text
                        }

                        MouseArea {
                            id: sceneMouseArea

                            anchors.fill: parent

                            acceptedButtons: Qt.RightButton

                            property bool hasMoved: false

                            onPressed: {
                                // Disable interaction so that the queue is not
                                // moving while we move the scenes around
                                flickable.interactive = false
                            }

                            onPositionChanged: {
                                var deltaX = mouse.x / queue.zoomProportion

                                var nextScene = queuemanager.queue[index + 1]
                                var previousScene = queuemanager.queue[index - 1]

                                if (previousScene && deltaX < -(queue.sceneWidth * 0.25)) {
                                    queuemanager.swap(index, index - 1)
                                }
                                else if (nextScene && deltaX > queue.sceneWidth + queue.sceneWidth * 0.25) {
                                    queuemanager.swap(index, index + 1)
                                }

                                hasMoved = true;
                            }

                            onReleased: {
                                if (!hasMoved) {
                                    queuemanager.selectScene(index);
                                }
                                hasMoved = false;

                                flickable.interactive = true
                            }

                            onWheel: {
                                adjustZoom(wheel)
                            }
                        }
                    }
                }
            }
        }
    }
}
