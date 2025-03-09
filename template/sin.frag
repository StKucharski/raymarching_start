#version 330 core

uniform float u_time;
uniform vec2 u_resolution;
out vec4 FragColor;

void main() {
    vec2 uv = (gl_FragCoord.xy / u_resolution.xy) * 2.0 - 1.0;
    uv.x *= u_resolution.x / u_resolution.y; // Aspect fix

    float color = 0.5 + 0.5 * sin(u_time + uv.x * 5.0);
    FragColor = vec4(vec3(color), 1.0);
}

