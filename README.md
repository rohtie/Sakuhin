# Shikaku

Software for live coding visuals with OpenGL shaders


#### Features

* Small vertical GUI so that you can have your favorite text editor next to it
* Channel input to shader
    * Textures
    * Shaders
    * FFT analysed microphone
    * Webcam

* Integrated MIDI controller passing its values to the shader
* Integration with [Splash](https://github.com/paperManu/splash) for advanced projection mapping

#### Technology

The UI is built with Qt Quick and has a C++ backend handling the OpenGL.
