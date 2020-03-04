#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;

uniform mat4 mvpMatrix;
uniform float time;

out vec2 uv;

mat2 rotate(float a) {
    return mat2(-sin(a), cos(a),
                 cos(a), sin(a));
}

void main() {
    vec3 p = position;

    gl_Position = mvpMatrix * vec4(p, 1.0);
    uv = vertexUV;
}
