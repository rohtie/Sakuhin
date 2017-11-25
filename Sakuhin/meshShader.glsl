#version 450 core

uniform sampler2D mainTexture;

in vec2 uv;
out vec4 outputColor;

void main() {
    outputColor = texture(mainTexture, uv);
}
