#version 330 core

layout(location = 0) in vec4 position;
layout(location = 2) in vec3 tex_cords;

uniform mat4 wvp;

out vec2 uv;
void main() {
    gl_Position = wvp * position;
    uv =  tex_cords.xy;
}
