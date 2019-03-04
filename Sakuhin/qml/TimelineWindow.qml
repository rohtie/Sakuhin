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

    property double zoomProportion: 1.0 / 3.0
    property double markerPosition: 0
    property double songDuration: 6250

    property double loopAreaStart: 0
    property double loopAreaLength: 0

    property int dragHandleWidth: 20

    function adjustZoom(wheel){
        var previousZoomProportion = timeline.zoomProportion

        var newZoomProportion = previousZoomProportion + (wheel.angleDelta.y / 120) * 0.04

        if (newZoomProportion < 0.0135) {
            newZoomProportion = previousZoomProportion
        }

        timeline.zoomProportion = newZoomProportion
    }

    function mouseToTimeline(mouse) {
        return Math.round((mouse.x + flickable.visibleArea.xPosition * flickable.contentWidth) / timeline.zoomProportion)
    }

    function adjustMarker(mouse) {
        timeline.markerPosition = mouseToTimeline(mouse)
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
        acceptedButtons: Qt.MiddleButton

        width: timeline.width
        height: timeline.height

        onPressed: {
            timeline.loopAreaStart = mouseToTimeline(mouse)
            timeline.loopAreaLength = 0
        }

        onReleased: {
            timeline.markerPosition = timeline.loopAreaStart
        }

        onPositionChanged: {
            timeline.loopAreaLength = Math.max(mouseToTimeline(mouse) - timeline.loopAreaStart, 0)
        }
    }

    Flickable {
        id: flickable

        y: 30

        width: timeline.width
        height: timeline.height - 30

        contentWidth: songIndicator.width

        Item {
            Item {
                id: songIndicator
                x: 0
                y: 0
                width: timeline.songDuration * timeline.zoomProportion

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
                    model: dummyDummModel

                    Rectangle {
                        id: scene

                        width: model.length * timeline.zoomProportion
                        height: parent.height

                        color: "#69697b"

                        border.width: 1
                        border.color: "#111117"

                        radius: 2

                        Text {
                            height: parent.height
                            text: model.name
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

                            onPressed: {
                                // Disable interaction so that the timeline is not
                                // moving while we move the scenes around
                                flickable.interactive = false

                                model.originalLength = model.length

                                if (mouse.x < timeline.dragHandleWidth) {
                                    sceneMouseArea.isAdjustingLeft = true

                                    var previousScene = dummyDummModel.get(index - 1)
                                    if (previousScene) {
                                        previousScene.originalLength = previousScene.length
                                    }
                                }
                                else if (mouse.x > (model.length * timeline.zoomProportion) - timeline.dragHandleWidth) {
                                    sceneMouseArea.isAdjustingRight = true

                                    var nextScene = dummyDummModel.get(index + 1)
                                    if (nextScene) {
                                        nextScene.originalLength = nextScene.length
                                    }
                                }
                                else {
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
                                var deltaX = mouse.x / timeline.zoomProportion

                                var nextScene = dummyDummModel.get(index + 1)
                                var previousScene = dummyDummModel.get(index - 1)

                                if (isAdjustingLeft) {
                                    adjustSizeLeft(model, previousScene, deltaX)
                                }
                                else if (isAdjustingRight) {
                                    adjustSizeRight(model, nextScene, deltaX)
                                }
                                else if (isSwapping) {

                                    if (previousScene && deltaX < -(previousScene.length * 0.25)) {
                                        dummyDummModel.move(index - 1, index, 1)
                                    }
                                    else if (nextScene && deltaX > model.length + nextScene.length * 0.25) {
                                        dummyDummModel.move(index + 1, index, 1)
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
                x: timeline.loopAreaStart * timeline.zoomProportion
                y: -2
                color: "#ffff00"
                width: timeline.loopAreaLength * timeline.zoomProportion
                height: 2
            }

            Rectangle {
                x: timeline.markerPosition * timeline.zoomProportion
                y: -10
                color: "#fff"
                width: 1
                height: timeline.height + 10
            }
        }
    }

    // Dummy model for debugging purposes
    ListModel {
        id: dummyDummModel

        ListElement {
            name: "Intro"
            length: 1500
            originalLength: 1500
        }

        ListElement {
            name: "Landscape"
            length: 150
            originalLength: 150
        }

        ListElement {
            name: "Rolling trees"
            length: 1500
            originalLength: 1500
        }

        ListElement {
            name: "Geometric balls"
            length: 700
            originalLength: 700
        }

        ListElement {
            name: "Outro"
            length: 1200
            originalLength: 1200
        }
    }

}