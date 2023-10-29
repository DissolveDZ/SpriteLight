#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int cur_char;
uniform int resolution;

void main()
{
    vec4 viewpos = view * model * vec4(aPos, 1.0);
    gl_Position = projection * viewpos;
	// float res = float(resolution);  // Convert resolution to a float
	TexCoords = aTexCoords;
    // TexCoords = vec2((aTexCoords.x + float(cur_char % resolution)) / res, (aTexCoords.y + float(cur_char / resolution)) / res);
}