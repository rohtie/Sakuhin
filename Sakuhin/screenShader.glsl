#version 450 core

uniform vec2 resolution;
uniform vec2 centerAdjustment;

uniform sampler2D screenTexture;

out vec4 outputColor;

void main() {
    vec2 p = gl_FragCoord.xy;

    p -= centerAdjustment;
    p /= resolution;

    outputColor = texture(screenTexture, p) - (1. - step(max(max(p.y, 1. - p.y), max(p.x, 1. - p.x)), 1.));
}
