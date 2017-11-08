#include "shader.h"

Shader::Shader(int id, QString thumbnail) {
    this->id = id;
    this->thumbnail = thumbnail;
}

int Shader::lastFrame() {
    return fbo->texture();
}
