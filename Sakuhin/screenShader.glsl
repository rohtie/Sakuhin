#version 450 core

uniform vec2 resolution;
uniform sampler2D screenTexture;
uniform sampler2D screenTextureResolution;

out vec4 outputColor;

void main() {
    vec2 p = gl_FragCoord.xy;
    p /= resolution;

    outputColor = texture(screenTexture, p);
}
