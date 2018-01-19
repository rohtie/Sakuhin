import QtQuick 2.7
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import sakuhin.backend 1.0
import sakuhin.shadermanager 1.0
import sakuhin.audiomanager 1.0
import sakuhin.windowmanager 1.0
import Qt.labs.folderlistmodel 2.2
import QtGraphicalEffects 1.0

import "qml"

ApplicationWindow {
    id: root
    flags: Qt.FramelessWindowHint
    visible: true
    width: 256
    height: 512
    x: 0
    y: 0
    color: "#111117"
    title: "Sakuhin"

    BackEnd {
        id: backend
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

    Component.onCompleted: backend.createSession();

    ColumnLayout {
        id: column
        anchors.topMargin: 5
        clip: false
        anchors.bottomMargin: 30
        spacing: 10
        anchors.fill: parent

        Label {
            height: 25
            color: "#bbb"
            text: "⚙ Sakuhin 作品"
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            font.pointSize: 11
            font.family: "Tahoma"
            anchors.right: parent.right

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
            text: "Transitions"
            interactive: true

            area.onClicked: {
                transitionCreationMenu.x = x + mouse.x
                transitionCreationMenu.y = y + mouse.y
                transitionCreationMenu.open()
            }
        }

        ShaderView {
            id: transitionGridView
            model: shadermanager.transitionShaders
            contextArea.onClicked: transitionContextMenu.openAt(x + mouse.x, y + mouse.y)
            onShaderActivated: shadermanager.selectTransition(currentIndex)
            activeIndex: -1
            isViewPreviewed: !shadermanager.isPreviewingShader
        }

        SectionLabel {
            text: "Channels"
        }

        GridView {
            id: channel_view

            height: 45
            clip: true
            maximumFlickVelocity: 3000
            flickDeceleration: 1500
            boundsBehavior: Flickable.StopAtBounds
            snapMode: GridView.NoSnap
            cellHeight: 48
            cellWidth: 48

            anchors.left: parent.left
            anchors.leftMargin: 10

            anchors.right: parent.right
            anchors.rightMargin: 2.5

            onCountChanged: currentIndex = count - 1

            Connections {
                target: shader_grid_view
                onShaderActivated: {
                    channel_view.model = shadermanager.shaders[shader_grid_view.currentIndex].channels
                }
            }

            Connections {
                target: transitionGridView
                onShaderActivated: {
                    channel_view.model = shadermanager.transitionShaders[transitionGridView.currentIndex].channels
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

                property alias channelImage: channelImage

                background: Rectangle {
                    anchors.fill: parent

                    anchors.topMargin: 0
                    anchors.rightMargin: 5
                    anchors.leftMargin: 0
                    anchors.bottomMargin: 5

                    color: "#69697b"
                    radius: 2

                    Image {
                        id: channelImage

                        anchors.fill: parent
                        fillMode: Image.PreserveAspectCrop

                        source: model.modelData.thumbnail

                        layer.enabled: true
                        layer.effect: OpacityMask {
                            maskSource: Item {
                                width: channelImage.width
                                height: channelImage.height

                                Rectangle {
                                    anchors.fill: parent
                                    radius: 2
                                }
                            }
                        }
                    }
                }
            }
        }

        SectionLabel {
            text: ""
        }

        ListView {
            id: shaderSliders

            Layout.fillHeight: true

            anchors.left: parent.left
            anchors.leftMargin: 0

            anchors.right: parent.right
            anchors.rightMargin: 0

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

            Connections {
                target: transitionGridView
                onShaderActivated: {
                    shaderSliders.model = shadermanager.transitionShaders[transitionGridView.currentIndex].sliders
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
        font.family: "Tahoma"
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
                    shadermanager.createTransition(filePath)
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

                    GridView {
                        clip: true
                        maximumFlickVelocity: 3000
                        flickDeceleration: 1500
                        boundsBehavior: Flickable.StopAtBounds
                        snapMode: GridView.NoSnap
                        cellHeight: 123
                        cellWidth: 123

                        anchors.fill: parent
                        anchors.topMargin: 5
                        anchors.leftMargin: 5

                        id: hardware_grid

                        Item {
                            width: hardware_grid.cellWidth
                            height: hardware_grid.cellHeight

                            Button {
                                anchors.fill: parent
                                anchors.topMargin: 0
                                anchors.rightMargin: 5
                                anchors.leftMargin: 5
                                anchors.bottomMargin: 10

                                onClicked: audio_menu.open()

                                background: Rectangle {
                                    implicitWidth: 100
                                    implicitHeight: 40
                                    color: "#69697b"
                                    border.width: 0
                                    radius: 5

                                    Image {
                                        anchors.fill: parent
                                        source: "qrc:assets/audio_icon.jpg"

                                        layer.enabled: true
                                        layer.effect: OpacityMask {
                                            maskSource: Item {
                                                width: textureGrid.width
                                                height: textureGrid.height

                                                Rectangle {
                                                    anchors.fill: parent
                                                    radius: 5
                                                }
                                            }
                                        }
                                    }
                                }

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
                        cellHeight: 123
                        cellWidth: 123

                        anchors.fill: parent
                        anchors.topMargin: 5
                        anchors.leftMargin: 5

                        model: FolderListModel {
                            folder: "file:data/textures/"
                        }

                        delegate: Item {
                            width: textureGrid.cellWidth
                            height: textureGrid.cellHeight

                            Image {
                                anchors.fill: parent
                                anchors.topMargin: 0
                                anchors.rightMargin: 2.5
                                anchors.leftMargin: 2.5
                                anchors.bottomMargin: 5

                                asynchronous: true

                                fillMode: Image.PreserveAspectFit
                                source: fileURL

                                layer.enabled: true
                                layer.effect: OpacityMask {
                                    maskSource: Item {
                                        width: textureGrid.width
                                        height: textureGrid.height

                                        Rectangle {
                                            anchors.fill: parent
                                            radius: 5
                                        }
                                    }
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
                    }
                }

                Item {
                    id: shaderTab

                    GridView {
                        id: shaderGrid
                        clip: true
                        maximumFlickVelocity: 3000
                        flickDeceleration: 1500
                        boundsBehavior: Flickable.StopAtBounds
                        snapMode: GridView.NoSnap
                        cellHeight: 123
                        cellWidth: 123

                        anchors.fill: parent
                        anchors.topMargin: 5
                        anchors.leftMargin: 5

                        model: shadermanager.shaders

                        delegate: Item {
                            width: shaderGrid.cellWidth
                            height: shaderGrid.cellHeight

                            Image {
                                id: shaderImage
                                anchors.fill: parent
                                anchors.topMargin: 0
                                anchors.rightMargin: 5
                                anchors.leftMargin: 0
                                anchors.bottomMargin: 5

                                asynchronous: true
                                fillMode: Image.PreserveAspectCrop
                                source: model.modelData.thumbnail

                                layer.enabled: true
                                layer.effect: OpacityMask {
                                    maskSource: Item {
                                        width: shaderImage.width
                                        height: shaderImage.height

                                        Rectangle {
                                            anchors.fill: parent
                                            radius: 2
                                        }
                                    }
                                }
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
                }
            }

            TabBar {
                id: bar
                height: 40
                currentIndex: 1
                spacing: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                font.family: "Tahoma"
                font.pointSize: 11

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
                                font.family: "Tahoma"
                                verticalAlignment: Text.AlignVCenter
                            }

                            Switch {
                                id: projectionSwitch
                                text: qsTr("Projection mapping")
                                checked: model.modelData.isProjectionMapping
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
                                    checked: model.modelData.isVertical
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
                                    font.family: "Tahoma"
                                    anchors.right: parent.right
                                }

                                TextField {
                                    text: model.modelData.distanceFromObject
                                    inputMethodHints: Qt.ImhFormattedNumbersOnly

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
                                            value = 0
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
                                    font.family: "Tahoma"
                                    anchors.right: parent.right
                                }

                                TextField {
                                    text: model.modelData.projectorHeight
                                    inputMethodHints: Qt.ImhFormattedNumbersOnly

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
                                            value = 0
                                        }

                                        model.modelData.projectorHeight = value
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
                                    font.family: "Tahoma"
                                    anchors.right: parent.right
                                }

                                TextField {
                                    text: model.modelData.fieldOfView
                                    inputMethodHints: Qt.ImhFormattedNumbersOnly

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
                                            value = 0
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
                height: 40
                currentIndex: 1
                spacing: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                font.family: "Tahoma"
                font.pointSize: 9

                background: Rectangle {
                    color: "#111117"
                }

                TabForm {
                    text: qsTr("Session")
                    font.family: "Tahoma"
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
                    if (shadermanager.isPreviewingShader) {
                        shadermanager.shaders[shader_grid_view.currentIndex].sliders[shaderSliders.currentIndex].startEase(index)
                    }
                    else  {
                        shadermanager.transitionShaders[transitionGridView.currentIndex].sliders[shaderSliders.currentIndex].startEase(index)
                    }

                    sliderContextMenu.close()
                }
            }
        }
    }
}
