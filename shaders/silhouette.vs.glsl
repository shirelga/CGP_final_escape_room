#version 330

layout (location = 0) in vec4 position;
//layout (location = 1) in vec2 TexCoord;
layout (location = 1) in vec4 normal;

out vec3 WorldPos0;                                                                 

uniform mat4 wvp;
uniform mat4 world;
                                                                                    
void main()                                                                         
{                                                                                   
//    vec4 PosL   = vec4(position, 1.0);
    gl_Position = wvp * position;
    WorldPos0 = (world * position).xyz;
}
