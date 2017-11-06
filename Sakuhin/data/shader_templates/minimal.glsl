vec4 pixel(vec2 p) {
    p /= resolution;
    p -= .5;
    p.x *= resolution.x / resolution.y;

    return vec4(0.);
}
