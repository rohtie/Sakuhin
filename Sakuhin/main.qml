import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Templates 2.2
import sakuhin.backend 1.0

ApplicationWindow {
    id: root
    flags: Qt.FramelessWindowHint
    visible: true
    width: 382
    height: 748
    color: "#111117"
    title: "Sakuhin"

    BackEnd {
        id: backend
    }

    Component.onCompleted: backend.createSession();

    Column {
        id: column
        spacing: 10
        anchors.fill: parent

        Image {
            id: preview
            height: 215
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            fillMode: Image.PreserveAspectFit
            source: "tmp/XlscD4.jpg"
        }

        Image {
            id: spectrum
            height: 25
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            fillMode: Image.PreserveAspectFit
            source: "tmp/spectrum.jpg"
        }

        Label {
            id: shaders_label
            height: 15
            color: "#dddddd"
            text: "Shaders"
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            font.pointSize: 8
            font.family: "Tahoma"
            anchors.right: parent.right

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    shaders.visible = !shaders.visible
                    shaders_icon.text = shaders.visible ? '-' : '+'
                }
            }

            Label {
                id: shaders_icon
                x: 0
                y: 0
                height: 15
                color: "#dddddd"
                text: '-'
                horizontalAlignment: Text.AlignRight
                anchors.rightMargin: 0
                font.pointSize: 8
                anchors.left: parent.left
                anchors.right: parent.right
                font.family: "Tahoma"
                anchors.leftMargin: 0
            }
        }

        ScrollView {
            id: shaders
            height: 155
            contentHeight: flow1.height
            rightPadding: 0
            topPadding: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            antialiasing: false
            padding: 0
            leftPadding: 0
            clip: true

            Flow {
                id: flow1
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 0
                layoutDirection: Qt.LeftToRight
                flow: Flow.LeftToRight
                spacing: 10

                Image {
                    id: image30
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/MtXSzf.jpg"
                }

                Image {
                    id: image
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XlscD4.jpg"
                }

                Image {
                    id: image6
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XlscD4.jpg"
                }

                Image {
                    id: image1
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/llsczl.jpg"
                }

                Image {
                    id: image2
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/llXyWN.jpg"
                }

                Image {
                    id: image3
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XdjyR1.jpg"
                }

                Image {
                    id: image4
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XtsyWN.jpg"
                }


                Image {
                    id: image5
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XlsyDn.jpg"
                }


                Image {
                    id: image7
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/llsczl.jpg"
                }

                Image {
                    id: image8
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/llXyWN.jpg"
                }

                Image {
                    id: image9
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XdjyR1.jpg"
                }

                Image {
                    id: image10
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XtsyWN.jpg"
                }

                Image {
                    id: image11
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XlsyDn.jpg"
                }

                Image {
                    id: image16
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XtsyWN.jpg"
                }

                Image {
                    id: image12
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XlscD4.jpg"
                }

                Image {
                    id: image13
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/llsczl.jpg"
                }

                Image {
                    id: image14
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/llXyWN.jpg"
                }

                Image {
                    id: image15
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XdjyR1.jpg"
                }

                Image {
                    id: image19
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/llsczl.jpg"
                }


                Image {
                    id: image17
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XlsyDn.jpg"
                }

                Image {
                    id: image18
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XlscD4.jpg"
                }

                Image {
                    id: image24
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XlscD4.jpg"
                }


                Image {
                    id: image20
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/llXyWN.jpg"
                }

                Image {
                    id: image21
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XdjyR1.jpg"
                }

                Image {
                    id: image22
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XtsyWN.jpg"
                }

                Image {
                    id: image23
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XlsyDn.jpg"
                }


                Image {
                    id: image31
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/MtXSzf.jpg"
                }

                Image {
                    id: image25
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/llsczl.jpg"
                }

                Image {
                    id: image26
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/llXyWN.jpg"
                }

                Image {
                    id: image27
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XdjyR1.jpg"
                }

                Image {
                    id: image28
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XtsyWN.jpg"
                }

                Image {
                    id: image29
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XlsyDn.jpg"
                }
            }
        }

        Label {
            id: channels_label
            height: 15
            color: "#dddddd"
            text: 'Channels'
            font.pointSize: 8
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            font.family: "Tahoma"
            anchors.right: parent.right

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    channels.visible = !channels.visible
                    channels_icon.text = channels.visible ? '-' : '+'
                }
            }

            Label {
                id: channels_icon
                x: 0
                y: 0
                height: 15
                color: "#dddddd"
                text: '-'
                horizontalAlignment: Text.AlignRight
                anchors.rightMargin: 0
                font.pointSize: 8
                anchors.left: parent.left
                anchors.right: parent.right
                font.family: "Tahoma"
                anchors.leftMargin: 0
            }
        }

        Flow {
            id: channels
            width: 300
            height: 52
            clip: true
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            spacing: 10

            Rectangle {
                id: rectangle12
                width: 52
                height: 52
                color: "#69697b"
            }

            Rectangle {
                id: rectangle13
                width: 52
                height: 52
                color: "#69697b"
            }

            Rectangle {
                id: rectangle14
                width: 52
                height: 52
                color: "#69697b"
            }

            Rectangle {
                id: rectangle15
                width: 52
                height: 52
                color: "#69697b"
            }

            Rectangle {
                id: rectangle16
                width: 52
                height: 52
                color: "#69697b"
            }

            Rectangle {
                id: rectangle17
                width: 52
                height: 52
                color: "#69697b"
            }
        }

        Label {
            id: controller_label
            height: 15
            color: "#dddddd"
            text: "Controllers"
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            font.pointSize: 8
            font.family: "Tahoma"
            anchors.right: parent.right

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    controller.visible = !controller.visible
                    controller_icon.text = controller.visible ? '-' : '+'
                }
            }

            Label {
                id: controller_icon
                x: 0
                y: 0
                height: 15
                color: "#dddddd"
                text: '-'
                horizontalAlignment: Text.AlignRight
                anchors.rightMargin: 0
                font.pointSize: 8
                anchors.left: parent.left
                anchors.right: parent.right
                font.family: "Tahoma"
                anchors.leftMargin: 0
            }
        }

        RowLayout {
            id: controller
            height: 150
            spacing: 0
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10

            Slider {
                id: control
                width: 10
                height: 133
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                wheelEnabled: true
                orientation: Qt.Vertical

                property int sliderID: 0

                onValueChanged: {
                    backend.setSlider(sliderID, value)
                }

                background: Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    x: control.leftPadding
                    y: control.topPadding + control.availableHeight / 2 - height / 2
                    implicitWidth: 10
                    implicitHeight: 100
                    width: implicitWidth
                    height: control.availableHeight
                    radius: 2
                    color: "#69697b"

                    Rectangle {
                        anchors.bottom: parent.bottom

                        width: parent.width
                        height: (1. - control.visualPosition) * parent.height
                        color: "#21be2b"
                        radius: 2
                    }
                }
            }

            Slider {
                id: control1
                width: 10
                height: 133
                Layout.fillWidth: true
                value: 0.5
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                background: Rectangle {
                    x: control1.leftPadding
                    y: control1.topPadding + control1.availableHeight / 2 - height / 2
                    width: implicitWidth
                    height: control1.availableHeight
                    color: "#69697b"
                    radius: 2
                    Rectangle {
                        width: parent.width
                        height: (1. - control1.visualPosition) * parent.height
                        color: "#21be2b"
                        radius: 2
                        anchors.bottom: parent.bottom
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                    implicitWidth: 10
                    implicitHeight: 100
                }
                wheelEnabled: true
                orientation: Qt.Vertical
            }

            Slider {
                id: control2
                width: 10
                height: 133
                Layout.fillWidth: true
                value: 0.5
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                background: Rectangle {
                    x: control2.leftPadding
                    y: control2.topPadding + control2.availableHeight / 2 - height / 2
                    width: implicitWidth
                    height: control2.availableHeight
                    color: "#69697b"
                    radius: 2
                    Rectangle {
                        width: parent.width
                        height: (1. - control2.visualPosition) * parent.height
                        color: "#21be2b"
                        radius: 2
                        anchors.bottom: parent.bottom
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                    implicitWidth: 10
                    implicitHeight: 100
                }
                wheelEnabled: true
                orientation: Qt.Vertical
            }

            Slider {
                id: control3
                width: 10
                height: 133
                Layout.fillWidth: true
                value: 0.5
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                background: Rectangle {
                    x: control3.leftPadding
                    y: control3.topPadding + control3.availableHeight / 2 - height / 2
                    width: implicitWidth
                    height: control3.availableHeight
                    color: "#69697b"
                    radius: 2
                    Rectangle {
                        width: parent.width
                        height: (1. - control3.visualPosition) * parent.height
                        color: "#21be2b"
                        radius: 2
                        anchors.bottom: parent.bottom
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                    implicitWidth: 10
                    implicitHeight: 100
                }
                wheelEnabled: true
                orientation: Qt.Vertical
            }
        }
    }

    Label {
        id: info
        x: -5
        y: -8
        height: 15
        color: "#dddddd"
        text: qsTr("60 fps")
        horizontalAlignment: Text.AlignRight
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.rightMargin: 10
        font.pointSize: 8
        anchors.left: parent.left
        anchors.right: parent.right
        font.family: "Tahoma"
        anchors.leftMargin: 10
    }
}
