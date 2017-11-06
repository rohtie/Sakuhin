vec4 pixel(vec2 p) {
    float seconds = 15.;
    float transitionTime = mod(time / seconds, 1.);

    p /= resolution;
    p -= 0.5;

    float trans = smoothstep(0., transitionTime, length(p) - transitionTime * 0.5);
    return (texture(channel0, p) * trans) + (texture(channel1, p) * (1. - trans));
}
