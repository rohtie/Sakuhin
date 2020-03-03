#version 450 core

uniform vec2 resolution;
uniform sampler2D screenTexture;

out vec4 outputColor;

void main() {
    vec2 p = gl_FragCoord.xy / resolution.xy;

    float distortion = -.2 + .7 * (-1.0 * .5 + .5);
    distortion *= 1.5;
    float distortionCorrectness = 0.0;

    p = p * 2.0 - 1.0;

    float radius = p.x*p.x + p.y*p.y;
    p *= 1.0 + distortion * radius + distortionCorrectness * radius * radius;

    p = (p * .5 + .5);

    p *= 1.5;
    p -= vec2(0.25, 0.25);

    float scale = abs(distortion) < 1. ? 1.-abs(distortion) : 1. / (distortion+1.);

    p = p * scale - (scale * .5) + .5;
    vec3 color = (texture(screenTexture, p)).rgb;

    p = mix(p, p * scale - (scale * .5) + .5, 0.1);
    float r = texture(screenTexture, p).r;

    p = mix(p, p * scale - (scale * .5) + .5, 0.1);
    float g = texture(screenTexture, p).g;

    p = mix(p, p * scale - (scale * .5) + .5, 0.1);
    float b = texture(screenTexture, p).b;

    color += vec3(r, g, b).grb * 0.25;

    outputColor = vec4(color, 1.);
}
