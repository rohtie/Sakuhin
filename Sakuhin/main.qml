import QtQuick 2.7
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import sakuhin.backend 1.0
import sakuhin.sessionmanager 1.0
import sakuhin.shadermanager 1.0
import sakuhin.audiomanager 1.0
import sakuhin.windowmanager 1.0
import sakuhin.scenemanager 1.0
import sakuhin.queuemanager 1.0
import Qt.labs.folderlistmodel 2.2
import QtGraphicalEffects 1.0
import QtQuick.Window 2.2

import "qml"

ApplicationWindow {
    visible: true
    width: 1
    height: 1
    color: "#000000"

    Backend {
        id: backend
    }

    SessionManager {
        id: sessionmanager
    }

    ShaderManager {
        id: shadermanager
    }

    AudioManager {
        id: audiomanager
    }

    WindowManager {
        id: windowmanager
    }

    SceneManager {
        id: scenemanager
    }

    QueueManager {
        id: queuemanager
    }

    Window {
        id: root

        flags: Qt.FramelessWindowHint
        visible: true
        width: 256
        height: 400
        x: 0
        y: 656
        color: "#111117"
        title: "Sakuhin"

        Shortcut {
            sequence: "Ctrl+S"
            onActivated: sessionmanager.saveSession()
        }

        Shortcut {
            sequence: "Ctrl+e"
            onActivated: sessionmanager.exportSession()
        }

        Shortcut {
            sequence: "Ctrl+Right"
            onActivated: shadermanager.shaders[shader_grid_view.currentIndex].interruptQueueNext()
        }

        Shortcut {
            sequence: "Ctrl+Space"
            onActivated: shadermanager.shaders[shader_grid_view.currentIndex].toggleQueuePlay()
        }


        ColumnLayout {
            id: column

            clip: false
            spacing: 10

            anchors.fill: parent
            anchors.topMargin: 5
            anchors.bottomMargin: 30
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            Layout.fillWidth: true

            Label {
                height: 25

                color: "#bbb"
                text: "⚙ Sakuhin 作品"

                font.pointSize: 10
                font.family: "Arimo"

                MouseArea {
                    anchors.fill: parent
                    onClicked: settingsPopup.open()
                }
            }

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
                id: shader_grid_view
                model: shadermanager.shaders
                contextArea.onClicked: shaderContextMenu.openAt(x + mouse.x, y + mouse.y)
                onShaderActivated: shadermanager.selectShader(currentIndex)
                activeIndex: shadermanager.mainIndex
                isViewPreviewed: shadermanager.isPreviewingShader
            }

            SectionLabel {
                text: "Channels"
            }

            BalancedGridView {
                id: channel_view

                onCountChanged: currentIndex = count - 1

                Connections {
                    target: shader_grid_view
                    onShaderActivated: {
                        channel_view.model = shadermanager.shaders[shader_grid_view.currentIndex].channels
                    }
                }

                delegate: Button {
                    id: channel

                    width: channel_view.cellWidth
                    height: channel_view.cellHeight

                    onClicked: {
                        channel_view.currentIndex = index
                        channelPopup.activate(channel_view.currentIndex)
                    }

                    background: RoundImage {
                        path: model.modelData.thumbnail
                    }
                }
            }

            SectionLabel {
                text: "Sliders"
            }

            ListView {
                id: shaderSliders

                Layout.fillWidth: true
                Layout.fillHeight: true

                orientation: ListView.Horizontal

                delegate: SliderControllerForm {
                    width: shaderSliders.width / shaderSliders.count
                    value: model.modelData.value
                    onValueChanged: model.modelData.value = value

                    MouseArea {
                        anchors.fill: parent

                        acceptedButtons: Qt.RightButton
                        onClicked: {
                            shaderSliders.currentIndex = index
                            sliderContextMenu.openAt(x + mouse.x, y + mouse.y)
                        }
                    }
                }

                Connections {
                    target: shader_grid_view
                    onShaderActivated: {
                        shaderSliders.model = shadermanager.shaders[shader_grid_view.currentIndex].sliders
                    }
                }
            }
        }

        Label {
            id: info
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10

            height: 10
            color: "#dddddd"
            text: backend.performanceInformation
            horizontalAlignment: Text.AlignRight
            font.family: "Arimo"
            font.pointSize: 7
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
                        shadermanager.createShader(filePath)
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
            id: shaderContextMenu
            height: shaderContextList.contentHeight

            function openAt(x, y) {
                this.x = x
                this.y = y
                open()
            }

            onHeightChanged: {
                if (height > root.height) {
                    height = root.height
                }
            }

            background: Rectangle {
                implicitWidth: 256
                implicitHeight: 256
                anchors.fill: parent
                color: "#111117"
            }

            ListView {
                height: shaderContextMenu.height
                boundsBehavior: Flickable.StopAtBounds

                id: shaderContextList

                model: ListModel {
                    ListElement {
                        name: "Make current"
                    }
                }

                delegate: StyledMenuItem {
                    text: name

                    onClicked: {
                        shadermanager.makeCurrent(shader_grid_view.currentIndex)
                        shaderContextMenu.close()
                    }
                }
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

            Column {
                spacing: 0

                anchors.fill: parent

                Item {
                    width: parent.width
                    height: parent.height - bar.height

                    StackLayout {
                        visible: true
                        currentIndex: bar.currentIndex

                        anchors.fill: parent
                        anchors.topMargin: 5
                        anchors.leftMargin: 5

                        BalancedGridView {
                            id: hardware_grid

                            Item {
                                width: hardware_grid.cellWidth
                                height: hardware_grid.cellHeight

                                Button {
                                    anchors.fill: parent

                                    background: RoundImage {
                                        path: "qrc:assets/audio_icon.jpg"
                                    }

                                    onClicked: audio_menu.open()

                                    Menu {
                                        id: audio_menu
                                        height: audio_list.contentHeight

                                        onHeightChanged: {
                                            if (height > root.height) {
                                                height = root.height
                                            }
                                        }

                                        background: Rectangle {
                                            implicitWidth: 256
                                            implicitHeight: 256
                                            anchors.fill: parent
                                            color: "#111117"
                                        }

                                        ListView {
                                            height: audio_menu.height
                                            boundsBehavior: Flickable.StopAtBounds

                                            id: audio_list

                                            model: audiomanager.audioDevices

                                            delegate: StyledMenuItem {
                                                text: model.modelData.name

                                                onClicked: {
                                                    // TODO: Find a cleaner way to write this...
                                                    shadermanager.shaders[shader_grid_view.currentIndex].channels[channel_view.currentIndex].setAudioDevice(audiomanager.audioDevices[index])
                                                    audio_menu.close()
                                                    channelPopup.close()
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        BalancedGridView {
                            id: textureGrid

                            model: FolderListModel {
                                folder: "file:data/textures/"
                            }

                            delegate: Item {
                                width: textureGrid.cellWidth
                                height: textureGrid.cellHeight

                                RoundImage {
                                    path: fileURL
                                }

                                MouseArea {
                                    anchors.fill: parent

                                    onClicked: {
                                        var channelID = channelPopup.currentChannelID

                                        shadermanager.shaders[shader_grid_view.currentIndex].channels[channel_view.currentIndex].setTexture(filePath)
                                        channelPopup.close()
                                    }
                                }
                            }
                        }

                        BalancedGridView {
                            id: shaderGrid

                            model: shadermanager.shaders

                            delegate: Item {
                                width: shaderGrid.cellWidth
                                height: shaderGrid.cellHeight

                                RoundImage {
                                    path: model.modelData.thumbnail
                                }

                                MouseArea {
                                    anchors.fill: parent

                                    onClicked: {
                                        var channelID = channelPopup.currentChannelID

                                        shadermanager.shaders[shader_grid_view.currentIndex].channels[channel_view.currentIndex].setShader(shadermanager.shaders[index])
                                        channelPopup.close()
                                    }
                                }
                            }
                        }

                        BalancedGridView {
                            id: videoGrid

                            model: FolderListModel {
                                folder: "file:data/videos/"
                                showDirsFirst: true
                            }

                            delegate: Item {
                                width: videoGrid.cellWidth
                                height: videoGrid.cellHeight

                                RoundImage {
                                    path: fileIsDir ? "qrc:assets/thumbnail.jpg" : "qrc:assets/audio_icon.jpg"
                                }

                                Label {
                                    visible: fileIsDir

                                    anchors.fill: parent
                                    anchors.topMargin: 0
                                    anchors.rightMargin: 5
                                    anchors.leftMargin: 0
                                    anchors.bottomMargin: 5

                                    color: "#fff"
                                    text: fileName
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
                                        var channelID = channelPopup.currentChannelID

                                        shadermanager.shaders[shader_grid_view.currentIndex].channels[channel_view.currentIndex].setVideo(filePath)
                                        channelPopup.close()
                                    }
                                }
                            }
                        }
                    }
                }

                TabBar {
                    id: bar

                    Layout.fillHeight: true

                    currentIndex: 1
                    spacing: 0

                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0

                    font.family: "Arimo"
                    font.pointSize: 7

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
                    TabForm {
                        text: qsTr("Videos")
                    }
                }
            }
        }

        Popup {
            id: settingsPopup
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

            background: Rectangle {
                color: "#111117"
            }

            Rectangle {
                width: parent.width
                height: parent.height
                color: "#111117"
            }

            Column {
                spacing: 0
                anchors.fill: parent

                StackLayout {
                    width: parent.width
                    height: parent.height - settingsBar.height
                    visible: true
                    currentIndex: settingsBar.currentIndex

                    Item {
                        id: session

                        Rectangle {
                            width: parent.width
                            height: parent.height
                            color: "#0000ff"
                        }
                    }

                    Item {
                        id: windows

                        ListView {
                            id: windowList

                            anchors.fill: parent
                            boundsBehavior: Flickable.StopAtBounds
                            model: windowmanager.windows

                            delegate: Column {
                                id: windowSettingsColumn
                                topPadding: 40

                                Label {
                                    height: 20
                                    color: "#bbb"
                                    text: model.modelData.isMaster ? "Master window" : model.modelData.isPreview ? "Preview window" : "Slave window"
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    anchors.rightMargin: 10
                                    anchors.right: parent.right
                                    font.pointSize: 10
                                    font.family: "Arimo"
                                    verticalAlignment: Text.AlignVCenter
                                }

                                Switch {
                                    id: projectionSwitch
                                    text: qsTr("Projection mapping")
                                    checked: model.modelData.isProjectionMapping || false
                                    onClicked: model.modelData.isProjectionMapping = checked
                                    font.pointSize: 7

                                    indicator: Rectangle {
                                        implicitWidth: 20
                                        implicitHeight: 10
                                        x: parent.leftPadding
                                        y: parent.height / 2 - height / 2
                                        radius: 13
                                        color: "#666"
                                        border.width: 0

                                        Rectangle {
                                            width: 10
                                            height: 10
                                            x: projectionSwitch.checked ? parent.width - width : 0

                                            radius: 13
                                            color: "#bbb"
                                            border.width: 0
                                        }
                                    }

                                    contentItem: Text {
                                        text: parent.text
                                        font: parent.font
                                        opacity: 1.0
                                        color: "#bbb"
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        leftPadding: parent.indicator.width + parent.spacing
                                    }
                                }

                                Column {
                                    spacing: 10

                                    visible: projectionSwitch.checked

                                    Switch {
                                        id: verticalSwitch
                                        text: qsTr("Vertical")
                                        checked: model.modelData.isVertical || false
                                        onClicked: model.modelData.isVertical = checked
                                        font.pointSize: 7

                                        indicator: Rectangle {
                                            implicitWidth: 20
                                            implicitHeight: 10
                                            x: parent.leftPadding
                                            y: parent.height / 2 - height / 2
                                            radius: 13
                                            color: "#666"
                                            border.width: 0

                                            Rectangle {
                                                width: 10
                                                height: 10
                                                x: verticalSwitch.checked ? parent.width - width : 0

                                                radius: 13
                                                color: "#bbb"
                                                border.width: 0
                                            }
                                        }

                                        contentItem: Text {
                                            text: parent.text
                                            font: parent.font
                                            opacity: 1.0
                                            color: "#bbb"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                            leftPadding: parent.indicator.width + parent.spacing
                                        }
                                    }

                                    Label {
                                        height: 10
                                        color: "#bbb"
                                        text: "Distance to object (cm)"
                                        anchors.left: parent.left
                                        anchors.leftMargin: 10
                                        anchors.rightMargin: 10
                                        font.pointSize: 7
                                        font.family: "Arimo"
                                        anchors.right: parent.right
                                    }

                                    TextField {
                                        text: model.modelData.distanceFromObject || ''

                                        color: "#bbb"
                                        font.pointSize: 7

                                        background: Rectangle {
                                            implicitWidth: windowList.width
                                            implicitHeight: 10
                                            color: "#333"
                                            border.width: 0
                                        }

                                        onTextChanged: {
                                            var value = parseFloat(text)

                                            if (isNaN(value)) {
                                                return
                                            }

                                            model.modelData.distanceFromObject = value
                                        }
                                    }

                                    Label {
                                        height: 10
                                        color: "#bbb"
                                        text: "Projector height (cm)"
                                        anchors.left: parent.left
                                        anchors.leftMargin: 10
                                        anchors.rightMargin: 10
                                        font.pointSize: 7
                                        font.family: "Arimo"
                                        anchors.right: parent.right
                                    }

                                    TextField {
                                        text: model.modelData.projectorHeight || ''

                                        color: "#bbb"
                                        font.pointSize: 7

                                        background: Rectangle {
                                            implicitWidth: windowList.width
                                            implicitHeight: 10
                                            color: "#333"
                                            border.width: 0
                                        }

                                        onTextChanged: {
                                            var value = parseFloat(text)

                                            if (isNaN(value)) {
                                                return
                                            }

                                            model.modelData.projectorHeight = value
                                        }
                                    }

                                    Label {
                                        height: 10
                                        color: "#bbb"
                                        text: "Object height target (cm)"
                                        anchors.left: parent.left
                                        anchors.leftMargin: 10
                                        anchors.rightMargin: 10
                                        font.pointSize: 7
                                        font.family: "Arimo"
                                        anchors.right: parent.right
                                    }

                                    TextField {
                                        text: model.modelData.objectHeightTarget || ''

                                        color: "#bbb"
                                        font.pointSize: 7

                                        background: Rectangle {
                                            implicitWidth: windowList.width
                                            implicitHeight: 10
                                            color: "#333"
                                            border.width: 0
                                        }

                                        onTextChanged: {
                                            var value = parseFloat(text)

                                            if (isNaN(value)) {
                                                return
                                            }

                                            model.modelData.objectHeightTarget = value
                                        }
                                    }

                                    Label {
                                        height: 10
                                        color: "#bbb"
                                        text: "Projector field of view (degrees)"
                                        anchors.left: parent.left
                                        anchors.leftMargin: 10
                                        anchors.rightMargin: 10
                                        font.pointSize: 7
                                        font.family: "Arimo"
                                        anchors.right: parent.right
                                    }

                                    TextField {
                                        text: model.modelData.fieldOfView || ''

                                        color: "#bbb"
                                        font.pointSize: 7

                                        background: Rectangle {
                                            implicitWidth: windowList.width
                                            implicitHeight: 10
                                            color: "#333"
                                            border.width: 0
                                        }

                                        onTextChanged: {
                                            var value = parseFloat(text)

                                            if (isNaN(value)) {
                                                return
                                            }

                                            model.modelData.fieldOfView = value
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                TabBar {
                    id: settingsBar
                    height: 30
                    currentIndex: 1
                    spacing: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    font.family: "Arimo"
                    font.pointSize: 9

                    background: Rectangle {
                        color: "#111117"
                    }

                    TabForm {
                        text: qsTr("Session")
                        font.family: "Arimo"
                    }
                    TabForm {
                        text: qsTr("Windows")
                    }
                }
            }
        }

        Menu {
            id: sliderContextMenu
            height: sliderContextList.contentHeight

            function openAt(x, y) {
                this.x = x
                this.y = y
                open()
            }

            onHeightChanged: {
                if (height > root.height) {
                    height = root.height
                }
            }

            background: Rectangle {
                implicitWidth: 256
                implicitHeight: 256

                anchors.fill: parent

                color: "#111117"
            }

            ListView {
                height: sliderContextMenu.height
                boundsBehavior: Flickable.StopAtBounds

                id: sliderContextList

                model: backend.easingNames

                delegate: StyledMenuItem {
                    text: model.modelData

                    onClicked: {
                        shadermanager.shaders[shader_grid_view.currentIndex].sliders[shaderSliders.currentIndex].startEase(index)

                        sliderContextMenu.close()
                    }
                }
            }
        }

        // Dummy model for debugging purposes
        ListModel {
            id: dummyModel

            ListElement {
                attribute: ""
            }

            ListElement {
                attribute: ""
            }

            ListElement {
                attribute: ""
            }

            ListElement {
                attribute: ""
            }

            ListElement {
                attribute: ""
            }

            ListElement {
                attribute: ""
            }

            ListElement {
                attribute: ""
            }

            ListElement {
                attribute: ""
            }

            ListElement {
                attribute: ""
            }

            ListElement {
                attribute: ""
            }

            ListElement {
                attribute: ""
            }

            ListElement {
                attribute: ""
            }

            ListElement {
                attribute: ""
            }

            ListElement {
                attribute: ""
            }
        }
    }

    // TimelineWindow {
    // }

    // QueueWindow {
    // }
}
