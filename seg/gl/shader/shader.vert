R"(#version 330
uniform mat4 model_matrix;
uniform mat4 vp_matrix;

uniform int color_mode;
uniform vec4 uniform_color;

uniform float visualize_hue_from;
uniform float visualize_hue_to;
uniform float visualize_saturation;
uniform float visualize_value;

uniform float visualize_z_min;
uniform float visualize_z_max;

in vec3 vertex_pos_model;
in vec3 vertex_color_rgb;
in vec4 vertex_color_rgba;
in float vertex_intensity;

out vec4 fragment_color;

// "hsv2rgb_smooth"
// The MIT License
// Copyright © 2014 Inigo Quilez
// https://www.youtube.com/c/InigoQuilez
vec3 hsv2rgb_smooth( in vec3 hsv )
{
    vec3 rgb = clamp( abs(mod(hsv.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );

	rgb = rgb*rgb*(3.0-2.0*rgb); // cubic smoothing	

	return hsv.z * mix( vec3(1.0), rgb, hsv.y);
}

vec3 float2rgb(in float value, in float value_min, in float value_max)
{
    value = clamp(value, value_min, value_max);
    float hue = (value - value_min)/(value_max - value_min) * (visualize_hue_to - visualize_hue_from) + visualize_hue_from;
    return hsv2rgb_smooth(vec3(hue,visualize_saturation, visualize_value));
}

void main(){
    vec4 world_pos = model_matrix * vec4(vertex_pos_model,1);
    gl_Position = vp_matrix * world_pos;

    if(color_mode == 0)
    {
        fragment_color = uniform_color;
    }
    else if(color_mode == 1) // rgb
    {
        fragment_color = vec4(vertex_color_rgb,1);
    }
    else if(color_mode == 2) // rgba
    {
        fragment_color = vertex_color_rgba;
    }
    else if(color_mode == 3) // scalar
    {
        fragment_color = vec4(float2rgb(vertex_intensity,0,255),1);
    }
    else if(color_mode == 4) // z axis
    {
        fragment_color = vec4(float2rgb(world_pos.z, visualize_z_min, visualize_z_max),1);
    }
}
)"