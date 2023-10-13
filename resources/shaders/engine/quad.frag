#version 450 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;
in float TexIndex;

// textures as ubo in future
uniform sampler2D textures[32];

void main()
{
    FragColor = texture(textures[int(round(TexIndex))], TexCoords);
}