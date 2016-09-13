#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
uniform vec3 camera_pos;
uniform mat4 view, perspective, model;

out vec4 nnormal;
out vec4 viewer;
out vec4 pos;
void main()
{
    pos = perspective * view * model * position;
    gl_Position = pos;
//    gl_Position = position;
    nnormal = view * model * normal;
    viewer = vec4(camera_pos, 1.f) - pos;
//    nnormal = normal;
//    viewer = view;
}
