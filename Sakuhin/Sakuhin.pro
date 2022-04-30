QT += core
QT += quick
QT += multimedia
QT += serialport

# fftw3 for spectrum analysis of audio
LIBS += -lfftw3

# ffmpeg related libraries for recording shaders to video files
#LIBS += -lavcodec -lavformat -lavutil -lswscale

CONFIG += c++11

CONFIG(debug, debug|release) {
    DESTDIR = ../sakuhin-debug
}
CONFIG(release, debug|release) {
    DESTDIR = ../sakuhin-release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.u

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    backend.cpp \
    main.cpp \
#    videogroup.cpp \
#    videomanager.cpp \
#    videoplayer.cpp \
    window.cpp \
    shadermanager.cpp \
    shader.cpp \
    channel.cpp \
    audiomanager.cpp \
    audiodevice.cpp \
    windowmanager.cpp \
    slider.cpp \
    wavefrontobjectloader.cpp \
    qmlreloadmanager.cpp \
#    videorecorder.cpp \
    visualswindow.cpp \
    mappingwindow.cpp \
    meshvisualswindow.cpp \
    timelinewindow.cpp \
    queuewindow.cpp \
    scenemanager.cpp \
    queuemanager.cpp \
    scene.cpp \
    sessionmanager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    backend.h \
    #videogroup.h \
    #videomanager.h \
    #videoplayer.h \
    window.h \
    shadermanager.h \
    shader.h \
    channel.h \
    audiomanager.h \
    audiodevice.h \
    #objloader.h \
    windowmanager.h \
    slider.h \
    wavefrontobjectloader.h \
    qmlreloadmanager.h \
    #videorecorder.h \
    visualswindow.h \
    mappingwindow.h \
    meshvisualswindow.h \
    timelinewindow.h \
    queuewindow.h \
    scenemanager.h \
    queuemanager.h \
    scene.h \
    sessionmanager.h

copydata.commands = $(COPY_DIR) $$PWD/data $$DESTDIR/data
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata


QDEP_DEPENDS += Skycoder42/QHotkey
!load(qdep):error("Failed to load qdep feature")
