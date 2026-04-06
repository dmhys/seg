R"(#version 330

out vec3 near_point;
out vec3 far_point;

uniform mat4 inv_vp_matrix;

// fullscreen quad vertices (two triangles)
const vec3 quad[6] = vec3[](
    vec3(-1, -1, 0), vec3(1, -1, 0), vec3(1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(-1, 1, 0)
);

vec3 unproject(vec3 ndc) {
    vec4 world = inv_vp_matrix * vec4(ndc, 1.0);
    return world.xyz / world.w;
}

void main() {
    vec3 p = quad[gl_VertexID];
    near_point = unproject(vec3(p.xy, 0.0));
    far_point  = unproject(vec3(p.xy, 1.0));
    gl_Position = vec4(p, 1.0);
}
)"
