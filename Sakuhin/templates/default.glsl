vec4 pixel(vec2 p) {
    p /= resolution;
    p -= 0.5;
    p.x *= resolution.x / resolution.y;

    float circle = smoothstep(0.005, 0., length(p) - 0.3);
    float bg = 1. - circle;

    return vec4(bg  * p.y + circle * abs(p.x * 4.5), bg * 0.5 + circle * p.y * 2., bg * 0.12 + circle, 1.);
}
