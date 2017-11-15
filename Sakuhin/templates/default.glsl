vec4 pixel(vec2 p) {
    p /= resolution;

    vec2 q = p;

    p -= 0.5;
    p.x *= resolution.x / resolution.y;

    p.x -= sin(p.y * (4. + 10. * slider[1]) + time) * 0.5;

    float circle = smoothstep(0.005, 0., length(p) - 0.3 - slider[0]);
    float bg = 1. - circle;

    vec4 col = vec4(
        bg  * p.y + circle * abs(p.x * 4.5 * slider[2]),
        bg * 0.5 + circle * p.y * 2. * slider[3],
        bg * 0.12 + circle,
        1.
    );

    return texture(channel0, q) * 0.99 + col * 0.01;
}
