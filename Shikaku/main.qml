import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Templates 2.2

ApplicationWindow {
    id: applicationWindow
    flags: Qt.FramelessWindowHint
    visible: true
    width: 320
    height: 748
    color: "#191919"
    title: "Shikaku"







    Column {
        id: column
        spacing: 10
        anchors.fill: parent

        Image {
            id: preview
            height: 180
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            fillMode: Image.PreserveAspectFit
            source: "tmp/XlscD4.jpg"
        }

        Label {
            id: shaders_label
            height: 15
            color: "#dddddd"
            text: qsTr("Shaders")
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            font.pointSize: 8
            font.family: "Tahoma"
            anchors.right: parent.right
        }

        ScrollView {
            id: shaders
            height: 155
            contentHeight: 362
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
                    id: image
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
                    id: image6
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XlscD4.jpg"
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
                    id: image16
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XtsyWN.jpg"
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
                    id: image19
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/llsczl.jpg"
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
                    id: image24
                    width: 52
                    height: 52
                    fillMode: Image.PreserveAspectCrop
                    source: "tmp/XlscD4.jpg"
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
            text: qsTr("Channels")
            font.pointSize: 8
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            font.family: "Tahoma"
            anchors.right: parent.right
        }

        Flow {
            id: channels
            width: 300
            height: 114
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
                color: "#747474"
            }

            Rectangle {
                id: rectangle13
                width: 52
                height: 52
                color: "#747474"
            }

            Rectangle {
                id: rectangle14
                width: 52
                height: 52
                color: "#747474"
            }

            Rectangle {
                id: rectangle15
                width: 52
                height: 52
                color: "#747474"
            }

            Rectangle {
                id: rectangle16
                width: 52
                height: 52
                color: "#747474"
            }

            Rectangle {
                id: rectangle17
                width: 52
                height: 52
                color: "#747474"
            }

            Rectangle {
                id: rectangle18
                width: 52
                height: 52
                color: "#747474"
            }

            Rectangle {
                id: rectangle19
                width: 52
                height: 52
                color: "#747474"
            }

            Rectangle {
                id: rectangle20
                width: 52
                height: 52
                color: "#747474"
            }

            Rectangle {
                id: rectangle21
                width: 52
                height: 52
                color: "#747474"
            }
        }





    }








}
