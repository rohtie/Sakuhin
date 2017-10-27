import QtQuick 2.7
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Templates 2.2
import sakuhin.backend 1.0
import "qml"

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

            ChannelForm {

                channel.onClicked: channelPopup.open()
            }

            ChannelForm {

                channel.onClicked: channelPopup.open()
            }

            ChannelForm {

                channel.onClicked: channelPopup.open()
            }

            ChannelForm {

                channel.onClicked: channelPopup.open()
            }

            ChannelForm {

                channel.onClicked: channelPopup.open()
            }

            ChannelForm {

                channel.onClicked: channelPopup.open()
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

            SliderControllerForm {
                id: slider0
                onValueChanged: backend.setSlider(0, value)
            }

            SliderControllerForm {
                id: slider1
                onValueChanged: backend.setSlider(1, value)
            }

            SliderControllerForm {
                id: slider2
                onValueChanged: backend.setSlider(2, value)
            }

            SliderControllerForm {
                id: slider3
                onValueChanged: backend.setSlider(3, value)
            }
        }
    }

    Label {
        id: info
        x: -5
        y: -8
        height: 15
        color: "#dddddd"
        text: backend.performanceInformation
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

    Popup {
        id: channelPopup
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape

        Rectangle {
            width: parent.width
            height: parent.height

            color: "#111117"
        }
    }
}
