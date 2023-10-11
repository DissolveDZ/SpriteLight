#pragma once

char *ReadTextFile(char *path);

// Function to format the shader uniform string
// and return a dynamically allocated string.
static char* FormatShaderUniform(const char* uniform_name, int index);

typedef struct Shader
{
    unsigned int ID;
} Shader;

Shader LoadShader(const char *vertex_name, const char *fragment_name);

void UseShader(Shader shader);

void SetShaderBool(int Shader_ID, const char *name, bool value);

void SetShaderInt(int Shader_ID, const char *name, int value);

void SetShaderFloat(int Shader_ID, const char *name, float value);

void SetShaderMat4(int Shader_ID, const char *name, mat4 matrix);

void SetShaderVec2(int Shader_ID, const char *name, vec2 vector);

void SetShaderVec3(int Shader_ID, const char *name, vec3 vector);

void SetShaderVec4(int Shader_ID, const char *name, vec4 value);

void SetShaderVec3v(int Shader_ID, const char *name, vec3 *vector, int amount);