import QtQuick 2.7
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Templates 2.2 as T
import QtQuick.Controls 2.2
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

        T.ScrollView {
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
        x: 0
        y: 0
        height: 15
        color: "#dddddd"
        text: backend.performanceInformation
        horizontalAlignment: Text.AlignRight
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottomMargin: 10
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        font.family: "Tahoma"
        font.pointSize: 8
    }

    Popup {
        id: channelPopup
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        rightPadding: 0
        leftPadding: 0
        bottomPadding: 0
        topPadding: 0
        spacing: 0
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape

        background: Rectangle {
            color: "#111117"
        }

        Rectangle {
            id: bg
            width: parent.width
            height: parent.height
            color: "#111117"
        }

        Column {
            spacing: 0
            anchors.fill: parent

            StackLayout {
                width: parent.width
                height: parent.height - bar.height
                visible: true
                currentIndex: bar.currentIndex

                Item {
                    id: hardwareTab

                    Rectangle {
                        x: -9
                        y: 6
                        width: parent.width
                        height: parent.height
                        color: "#ff0000"
                    }
                }
                Item {
                    id: textureTab

                    T.ScrollView {
                        id: scrollView
                        anchors.topMargin: 10
                        anchors.bottomMargin: 10
                        contentHeight: flow2.height
                        clip: true
                        anchors.fill: parent

                        Flow {
                            id: flow2
                            anchors.rightMargin: 10
                            anchors.leftMargin: 10
                            anchors.topMargin: 0
                            anchors.right: parent.right
                            anchors.left: parent.left
                            anchors.top: parent.top
                            flow: Flow.LeftToRight
                            Image {
                                id: image32
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/MtXSzf.jpg"
                            }

                            Image {
                                id: image33
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image34
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image35
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image36
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image37
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image38
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image39
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }

                            Image {
                                id: image40
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image41
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image42
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image43
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image44
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }

                            Image {
                                id: image45
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image46
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image47
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image48
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image49
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image50
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image51
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }

                            Image {
                                id: image52
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image53
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image54
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image55
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image56
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image57
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }

                            Image {
                                id: image58
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/MtXSzf.jpg"
                            }

                            Image {
                                id: image59
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image60
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image61
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image62
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image63
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }

                            Image {
                                id: image64
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/MtXSzf.jpg"
                            }

                            Image {
                                id: image65
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image66
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image67
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image68
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image69
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image70
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image71
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }

                            Image {
                                id: image72
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image73
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image74
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image75
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image76
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }

                            Image {
                                id: image77
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image78
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image79
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image80
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image81
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image82
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image83
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }

                            Image {
                                id: image84
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image85
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image86
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image87
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image88
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image89
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }

                            Image {
                                id: image90
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/MtXSzf.jpg"
                            }

                            Image {
                                id: image91
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image92
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image93
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image94
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image95
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }

                            Image {
                                id: image96
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/MtXSzf.jpg"
                            }

                            Image {
                                id: image97
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image98
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image99
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image100
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image101
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image102
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image103
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }

                            Image {
                                id: image104
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image105
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image106
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image107
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image108
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }

                            Image {
                                id: image109
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image110
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image111
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image112
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image113
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image114
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image115
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }

                            Image {
                                id: image116
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image117
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlscD4.jpg"
                            }

                            Image {
                                id: image118
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image119
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image120
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image121
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }

                            Image {
                                id: image122
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/MtXSzf.jpg"
                            }

                            Image {
                                id: image123
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llsczl.jpg"
                            }

                            Image {
                                id: image124
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/llXyWN.jpg"
                            }

                            Image {
                                id: image125
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XdjyR1.jpg"
                            }

                            Image {
                                id: image126
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XtsyWN.jpg"
                            }

                            Image {
                                id: image127
                                width: 52
                                height: 52
                                fillMode: Image.PreserveAspectCrop
                                source: "tmp/XlsyDn.jpg"
                            }
                            spacing: 10
                            layoutDirection: Qt.LeftToRight
                        }
                    }
                }
                Item {
                    id: shaderTab

                    Rectangle {
                        width: parent.width
                        height: parent.height
                        color: "#0000ff"
                    }
                }
            }

            TabBar {
                id: bar
                height: 40
                currentIndex: 1
                spacing: 1
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0

                background: Rectangle {
                    color: "#111117"
                }

                TabForm {
                    text: qsTr("Hardware")
                }
                TabForm {
                    text: qsTr("Textures")
                }
                TabForm {
                    text: qsTr("Shaders")
                }
            }

        }

    }

}
