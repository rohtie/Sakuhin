#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;

uniform mat4 mvpMatrix;
uniform float time;

out vec2 uv;

void main() {
    gl_Position = mvpMatrix * vec4(position, 1.0);
    uv = vertexUV;
}
