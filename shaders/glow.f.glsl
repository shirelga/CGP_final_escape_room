#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 uv;

void main() {
    FragColor = vec4(0.9765625, 0.91796875, 0.83984375, 1.f);
    float brightness = dot(FragColor.rgb, vec3(0.8, 0.8, 0.8));
    if(brightness > 1.f)
    {
        BrightColor = vec4(FragColor.rgb, 1.0);
    }
}
