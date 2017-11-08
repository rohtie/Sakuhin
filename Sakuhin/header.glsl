#version 450 core

uniform vec2 resolution;
uniform float time;
uniform float slider[4];
uniform sampler2D channel0;
uniform sampler2D channel1;
uniform sampler2D channel2;
uniform sampler2D channel3;
uniform sampler2D channel4;

out vec4 outputColor;
