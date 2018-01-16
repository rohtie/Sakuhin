vec4 pixel(vec2 p) {
    float seconds = 15.;
    float transitionTime = mod(time / seconds, 1.);

    p /= resolution;
    vec2 q = p;
    p -= 0.5;

    float trans = smoothstep(0., transitionTime, length(p) - transitionTime * 0.5 * texture(channel2, q).r);
    return (texture(channel0, q) * trans) + (texture(channel1, q) * (1. - trans));
}
