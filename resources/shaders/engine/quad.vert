#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in float aTexIndex;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec2 TexCoords;
out float TexIndex;

void main()
{
    TexIndex = aTexIndex;
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos.xy, 0, 1.0);
}