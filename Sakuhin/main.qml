import QtQuick 2.7
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import sakuhin.backend 1.0
import sakuhin.shadermanager 1.0
import Qt.labs.folderlistmodel 2.2
import QtGraphicalEffects 1.0

import "qml"

ApplicationWindow {
    id: root
    flags: Qt.FramelessWindowHint
    visible: true
    width: 256
    height: 494
    color: "#111117"
    title: "Sakuhin"

    BackEnd {
        id: backend
    }

    ShaderManager {
        id: shadermanager
    }

    Component.onCompleted: backend.createSession();

    Column {
        id: column
        anchors.topMargin: 10
        clip: false
        anchors.bottomMargin: 30
        spacing: 10
        anchors.fill: parent

        SectionLabel {
            text: "Shaders"
            interactive: true

            area.onClicked: {
                shaderCreationMenu.x = x + mouse.x
                shaderCreationMenu.y = y + mouse.y
                shaderCreationMenu.open()
            }
        }

        ShaderView {
            model: shadermanager.shaders
        }

        SectionLabel {
            text: "Transitions"
            interactive: true

            area.onClicked: {
                transitionCreationMenu.x = x + mouse.x
                transitionCreationMenu.y = y + mouse.y
                transitionCreationMenu.open()
            }
        }

        ShaderView {
            model: shadermanager.transitionShaders
            contextArea.onClicked: transitionContextMenu.openAt(x + mouse.x, y + mouse.y)
        }

        SectionLabel {
            text: "Channels"
        }

        Flow {
            id: channels
            height: 48
            clip: true
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            spacing: 5

            Repeater {
                id: channelRepeater
                model: 5

                ChannelForm {
                    channel.onClicked: channelPopup.activate(index)
                }
            }
        }

        SectionLabel {
            text: "Controllers"
        }

        RowLayout {
            id: controller
            spacing: 0
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10

            Repeater {
                model: 4

                SliderControllerForm {
                    onValueChanged: backend.setSlider(index, value)
                }
            }
        }
    }

    Image {
        id: spectrum
        x: 10
        y: 225
        height: 35
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        fillMode: Image.PreserveAspectCrop
        source: "tmp/spectrum.jpg"

        Label {
            id: info
            x: 10
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
    }


    Menu {
        id: shaderCreationMenu
        height: shaderMenuList.contentHeight

        ListView {
            id: shaderMenuList

            anchors.fill: parent

            interactive: false

            model: FolderListModel {
                folder: "file:data/shader_templates/"
                nameFilters: ["*.glsl"]
            }

            delegate: StyledMenuItem {
                text: fileBaseName

                onClicked: {
                    shadermanager.createShader(fileURL)
                    shaderCreationMenu.close()
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            color: "#111117"
        }
    }

    Menu {
        id: transitionCreationMenu
        height: transitionCreationList.contentHeight

        ListView {
            id: transitionCreationList

            anchors.fill: parent

            interactive: false

            model: FolderListModel {
                folder: "file:data/transition_templates/"
                nameFilters: ["*.glsl"]
            }

            delegate: StyledMenuItem {
                text: fileBaseName

                onClicked: {
                    shadermanager.createTransition(fileURL)
                    transitionCreationMenu.close()
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            color: "#111117"
        }
    }

    Menu {
        id: transitionContextMenu
        height: transitionContextList.contentHeight

        function openAt(x, y) {
            this.x = x
            this.y = y
            open()
        }

        ListView {
            id: transitionContextList

            anchors.fill: parent
            interactive: false

            model: ListModel {
                ListElement {
                    transitionTime: 1
                }
                ListElement {
                    transitionTime: 5
                }
                ListElement {
                    transitionTime: 10
                }
                ListElement {
                    transitionTime: 20
                }
                ListElement {
                    transitionTime: 30
                }
            }

            delegate: StyledMenuItem {
                text: transitionTime + " sec"

                onClicked: {
                    transitionContextMenu.close()
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            color: "#111117"
        }
    }

    Popup {
        id: channelPopup
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        visible: false
        rightPadding: 0
        leftPadding: 0
        bottomPadding: 0
        topPadding: 0
        spacing: 0
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape

        property int currentChannelID: 0

        function activate(channelID) {
            currentChannelID = channelID
            channelPopup.open()
        }

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
                        width: parent.width
                        height: parent.height
                        color: "#ff0000"
                    }
                }

                Item {
                    id: textureTab

                    GridView {
                        id: textureGrid
                        clip: true
                        maximumFlickVelocity: 3000
                        flickDeceleration: 1500
                        boundsBehavior: Flickable.StopAtBounds
                        snapMode: GridView.NoSnap
                        cellHeight: 124
                        cellWidth: 124

                        anchors.rightMargin: 5
                        anchors.leftMargin: 5
                        anchors.bottomMargin: 10
                        anchors.topMargin: 10
                        anchors.fill: parent

                        model: FolderListModel {
                            folder: "file:data/textures/"
                        }

                        delegate: Item {
                            width: textureGrid.cellWidth
                            height: textureGrid.cellHeight

                            Image {
                                anchors.fill: parent
                                anchors.topMargin: 0
                                anchors.rightMargin: 5
                                anchors.leftMargin: 5
                                anchors.bottomMargin: 10

                                asynchronous: true

                                fillMode: Image.PreserveAspectFit
                                source: fileURL

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        var channelID = channelPopup.currentChannelID

                                        backend.setChannel(channelID, BackEnd.Texture, filePath)
                                        channelRepeater.itemAt(channelID).channelImage.source = fileURL
                                        channelPopup.close()
                                    }
                                }
                            }
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
