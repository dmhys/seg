R"(#version 330

in vec3 near_point;
in vec3 far_point;

out vec4 frag_color;

uniform mat4 vp_matrix;
uniform vec4 grid_color;
uniform float grid_fade_distance;

float grid(vec2 pos, float scale) {
    vec2 coord = pos / scale;
    vec2 derivative = fwidth(coord);
    vec2 grid_aa = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid_aa.x, grid_aa.y);
    return 1.0 - min(line, 1.0);
}

void main() {
    // ray-plane intersection at z=0
    float t = -near_point.z / (far_point.z - near_point.z);

    if (t < 0.0) discard;

    vec3 world_pos = near_point + t * (far_point - near_point);

    // distance fade
    float dist = length(world_pos - near_point);
    float fade = 1.0 - smoothstep(0.0, grid_fade_distance, dist);

    if (fade <= 0.0) discard;

    // two-level grid: 1m thin + 10m thick
    float g1 = grid(world_pos.xy, 1.0) * 0.4;
    float g10 = grid(world_pos.xy, 10.0) * 0.7;
    float g = max(g1, g10);

    if (g <= 0.001) discard;

    // depth
    vec4 clip = vp_matrix * vec4(world_pos, 1.0);
    float depth = (clip.z / clip.w) * 0.5 + 0.5;
    gl_FragDepth = depth;

    frag_color = vec4(grid_color.rgb, grid_color.a * g * fade);
}
)"
