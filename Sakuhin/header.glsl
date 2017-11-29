#version 450 core

uniform vec2 resolution;
uniform float time;
uniform float slider0;
uniform float slider1;
uniform float slider2;
uniform float slider3;
uniform sampler2D channel0;
uniform sampler2D channel1;
uniform sampler2D channel2;
uniform sampler2D channel3;
uniform sampler2D channel4;

out vec4 outputColor;
