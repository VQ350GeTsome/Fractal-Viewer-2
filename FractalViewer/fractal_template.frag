#version 330 core

uniform vec2 center;
uniform float zoom;
uniform vec2 resolution;
uniform int maxIter;

vec2 iterate(vec2 z, vec2 c) {
    // __FRACTAL_FORMULA__
}

void main() {
    vec2 uv = gl_FragCoord.xy / resolution;
    vec2 c = center + (uv - 0.5) * zoom;

    vec2 z = vec2(0.0);
    int iter = 0;

    for (int i = 0; i < maxIter; i++) {
        z = iterate(z, c);
        if (dot(z, z) > 4.0) break;
        iter++;
    }

    float t = float(iter) / float(maxIter);
    gl_FragColor = vec4(vec3(t), 1.0);
}
