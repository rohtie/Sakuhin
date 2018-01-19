# Sakuhin

Software for live coding visuals with OpenGL shaders

![screenshot](https://i.imgur.com/5fx8M7v.png)

### Features

* Small vertical GUI so that you can have your favorite text editor next to it
* Several types of input to the shader
    * Textures
    * Other shaders
    * Audio spectrum analysis

* MIDI controller sliders
* Projection mapping based on a supplied 3D model

### Environment

The UI is built with Qt Quick and has a C++ backend handling the OpenGL.

### Build guide

Sakuhin is currently only tested on Arch Linux. But it should work on other linux distributions, mac osx and windows with minor tweaks.

You will need to install FFTW and the common QT libraries.
Downloading QT creator is probably the easiest way of building Sakuhin.
