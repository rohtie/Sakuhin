
float note(vec2 p, float pitch) {
    float r = length(p - vec2(0.01, -pitch)) - 0.04;
    r = min(r, max(abs(p.y) - pitch, abs(p.x - 0.04) - 0.01));
    r = max(r, p.y);

    return r;
}


vec4 pixel(vec2 p) {
    p /= resolution;
    p -= .5;
    p.x *= resolution.x / resolution.y;

    p *= 0.625;

    p.x += 0.11;
    p.y -= 0.14;


    float r = min(note(p, 0.25), note(p - vec2(0.2, 0.), 0.2));
    r = min(r, max(abs(p.y + 0.02) - 0.02, abs(p.x - 0.04 - 0.1) - 0.1));
    r = min(r, max(abs(p.y + 0.065) - 0.005, abs(p.x - 0.04 - 0.1) - 0.1));
    r = smoothstep(0., 0.005, r);


    vec4 fg = vec4(0.06666666666666667, 0.06666666666666667, 0.09019607843137255, 0.);
    vec4 bg = vec4(0.4117647058823529, 0.4117647058823529, 0.4823529411764706, 0.);

    return bg * r + fg * (1. - r);
}
