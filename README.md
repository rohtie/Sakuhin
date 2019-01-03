# Sakuhin

Software for live coding visuals with OpenGL shaders

![screenshot](https://i.imgur.com/5fx8M7v.png)

### Features

* Small vertical GUI so that you can have your favorite text editor next to it
* Several types of input to the shader
    * Textures
    * Other shaders
    * Audio spectrum analysis

* Projection mapping based on a supplied 3D model
    * Calibrate your models with `C`
* Recording of shader window to .mp4 with `R`

* MIDI controller sliders


### Environment

The UI is built with Qt Quick and has a C++ backend handling the OpenGL.


### Build guide

Sakuhin is currently only tested on Arch Linux. But it should work on other linux distributions, mac osx and windows with minor tweaks.


Install the following libraries:

QT5:

    libQt5Core
    libQt5Quick
    libQt5Qml
    libQt5Gui
    libQt5Multimedia

Opengl:

    libGL

FFTW:

    fftw

FFMPEG:

    libavcodec
    libavformat
    libavutil
    libswscale

Build the project like this:

    cd Sakuhin
    qmake && make -j8

Sakuhin will be compiled into `sakuhin-release` directory in the root directory.
