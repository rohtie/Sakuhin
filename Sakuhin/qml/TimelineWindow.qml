import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    id: timeline

    flags: Qt.FramelessWindowHint
    visible: true
    width: 1024
    height: 100
    x: 0
    y: 756
    color: "#111117"
    title: "Timeline"

    property double zoomProportion: 1.0 / 500.0

    property int dragHandleWidth: 20

    function adjustZoom(wheel){
        var previousZoomProportion = timeline.zoomProportion

        var newZoomProportion = previousZoomProportion + (wheel.angleDelta.y / 120) * 0.001

        if (newZoomProportion < 1.0 / 750.0) {
            newZoomProportion = previousZoomProportion
        }

        timeline.zoomProportion = newZoomProportion
    }

    function mouseToTimeline(mouse) {
        return Math.round((mouse.x + flickable.visibleArea.xPosition * flickable.contentWidth) / timeline.zoomProportion)
    }

    function adjustMarker(mouse) {
        scenemanager.skipTo(mouseToTimeline(mouse))
    }

    MouseArea {
        width: timeline.width
        height: timeline.height

        onPressed: {
            adjustMarker(mouse)
        }

        onPositionChanged: {
            adjustMarker(mouse)
        }


        onWheel: {
            adjustZoom(wheel)
        }
    }

    MouseArea {
        acceptedButtons: Qt.RightButton

        width: timeline.width
        height: timeline.height

        onPressed: {
            scenemanager.loopAreaStart = mouseToTimeline(mouse)
            scenemanager.loopAreaLength = 0
        }

        onReleased: {
            scenemanager.skipTo(scenemanager.loopAreaStart)
        }

        onPositionChanged: {
            scenemanager.loopAreaLength = Math.max(mouseToTimeline(mouse) - scenemanager.loopAreaStart, 0)
        }
    }

    Flickable {
        id: flickable

        y: 30

        width: timeline.width
        height: timeline.height - 30

        contentWidth: songIndicator.width

        focus: true

        Keys.onSpacePressed: {
            scenemanager.togglePlay()
        }

        Item {
            Item {
                id: songIndicator
                x: 0
                y: 0
                width: scenemanager.audioDuration * timeline.zoomProportion

                Rectangle {
                    x: 0
                    y: -2
                    color: "#21be2b"
                    width: songIndicator.width
                    height: 2
                }

                Rectangle {
                    x: 0
                    y: 0
                    color: "#013e0b"
                    width: songIndicator.width
                    height: flickable.height

                    MouseArea {
                        anchors.fill: parent

                        onWheel: {
                            adjustZoom(wheel)
                        }
                    }
                }
            }

            Row {
                id: sceneContainer

                height: flickable.height

                Repeater {
                    model: scenemanager.scenes

                    Rectangle {
                        id: scene

                        width: model.modelData.length * timeline.zoomProportion
                        height: parent.height

                        color: "#69697b"

                        border.width: 1
                        border.color: "#111117"

                        radius: 2

                        Text {
                            height: parent.height
                            text: model.modelData.name
                            leftPadding: 10
                            color: "#111117"
                            verticalAlignment: Text.AlignVCenter
                        }

                        MouseArea {
                            id: sceneMouseArea

                            anchors.fill: parent

                            acceptedButtons: Qt.RightButton

                            property bool isAdjustingLeft: false
                            property bool isAdjustingRight: false
                            property bool isSwapping: false
                            property bool hasSwappedLeft: false
                            property bool hasSwappedRight: false

                            property double grabX: 0

                            onPressed: {
                                // Disable interaction so that the timeline is not
                                // moving while we move the scenes around
                                flickable.interactive = false

                                model.modelData.originalLength = model.modelData.length

                                if (mouse.x < timeline.dragHandleWidth) {
                                    grabX = mouse.x

                                    sceneMouseArea.isAdjustingLeft = true

                                    var previousScene = scenemanager.scenes[index - 1]
                                    if (previousScene) {
                                        previousScene.originalLength = previousScene.length
                                    }
                                }
                                else if (mouse.x > (model.modelData.length * timeline.zoomProportion) - timeline.dragHandleWidth) {
                                    grabX = mouse.x - (model.modelData.length * timeline.zoomProportion)

                                    sceneMouseArea.isAdjustingRight = true

                                    var nextScene = scenemanager.scenes[index + 1]
                                    if (nextScene) {
                                        nextScene.originalLength = nextScene.length
                                    }
                                }
                                else {
                                    grabX = 0
                                    isSwapping = true
                                }
                            }

                            function adjustSizeRight(currentScene, nextScene, deltaX) {
                                currentScene.length = deltaX

                                if (nextScene) {
                                    nextScene.length = nextScene.originalLength - (currentScene.length - currentScene.originalLength)
                                }
                            }

                            function adjustSizeLeft(currentScene, previousScene, deltaX) {
                                adjustSizeRight(previousScene, currentScene, deltaX + previousScene.length)
                            }

                            onPositionChanged: {
                                var deltaX = (mouse.x - grabX) / timeline.zoomProportion

                                var currentScene = scenemanager.scenes[index]
                                var nextScene = scenemanager.scenes[index + 1]
                                var previousScene = scenemanager.scenes[index - 1]

                                if (isAdjustingLeft) {
                                    adjustSizeLeft(currentScene, previousScene, deltaX)
                                }
                                else if (isAdjustingRight) {
                                    adjustSizeRight(currentScene, nextScene, deltaX)
                                }
                                else if (isSwapping) {
                                    // TODO: figure out how to keep moving the scene after swap
                                    // as it is currently not possible due to complete refresh
                                    // of qml objects representing the scenes on scenesChanged signal
                                    if (previousScene && deltaX < -(previousScene.length * 0.25)) {
                                        scenemanager.swap(index, index - 1)
                                    }
                                    else if (nextScene && deltaX > model.modelData.length + nextScene.length * 0.25) {
                                        scenemanager.swap(index, index + 1)
                                    }
                                }
                            }

                            onReleased: {
                                sceneMouseArea.isAdjustingLeft = false
                                sceneMouseArea.isAdjustingRight = false
                                sceneMouseArea.isSwapping = false
                                sceneMouseArea.hasSwappedLeft = false
                                sceneMouseArea.hasSwappedRight = false

                                flickable.interactive = true
                            }

                            onWheel: {
                                adjustZoom(wheel)
                            }
                        }
                    }
                }
            }

            Rectangle {
                x: scenemanager.loopAreaStart * timeline.zoomProportion
                y: -2
                color: "#ffff00"
                width: scenemanager.loopAreaLength * timeline.zoomProportion
                height: 2
            }

            Rectangle {
                x: scenemanager.markerPosition * timeline.zoomProportion
                y: -10
                color: "#fff"
                width: 1
                height: timeline.height + 10
            }
        }
    }
}
