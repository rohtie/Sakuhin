uniform float time;
uniform vec2 resolution;

void main() {
    vec2 p = gl_FragCoord.xy;

    p /= resolution.xy;
    p -= 0.5;
    p.x *= resolution.x / resolution.y;

    gl_FragColor = vec4(vec3(smoothstep(0.35, 0.35 + sin(time * 0.5) * 0.1, length(p))) + vec3(abs(p.x), p.y, 0.5), 1.0);
}
