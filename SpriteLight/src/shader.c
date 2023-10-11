Shader LoadShader(const char *vertex_name, const char *fragment_name)
{
    const char *base_path = "resources/shaders/";
    int len = strlen(vertex_name) + strlen(fragment_name) + 2;
    char *combined_path = (char *)malloc(len);
    if (!combined_path)
        HandleError("Failed to allocate memory for combined_path");
    snprintf(combined_path, len, "%s+%s", vertex_name, fragment_name);
    combined_path[len - 1] = '\0';  // Ensure null termination
    Resource *resource = LoadResource(combined_path);
    if (resource && resource->data)
        return *(Shader *)resource->data;

    len += strlen(base_path);

    char *vertex_shader_path = (char *)malloc(len);
    char *fragment_shader_path = (char *)malloc(len);

    if (!vertex_shader_path || !fragment_shader_path)
        HandleError("Failed to allocate memory for shader paths");

    snprintf(vertex_shader_path, len, "%s%s", base_path, vertex_name);
    snprintf(fragment_shader_path, len, "%s%s", base_path, fragment_name);

    char *vertex_shader_source = ReadTextFile(vertex_shader_path);
    char *fragment_shader_source = ReadTextFile(fragment_shader_path);

    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    Shader shader;

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        HandleError("vertex shader compilation failed\n %s", info_log);
        exit(1);
    }

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);
        HandleError("fragment shader compilation failed\n %s", info_log);
        exit(1);
    }

    shader.ID = glCreateProgram();
    glAttachShader(shader.ID, vertex);
    glAttachShader(shader.ID, fragment);
    glLinkProgram(shader.ID);
    glGetProgramiv(shader.ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader.ID, 512, NULL, info_log);
        HandleError("shader linking failed\n %s", info_log);
        exit(1);
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free(vertex_shader_source);
    free(fragment_shader_source);

    if (resource)
        resource->data = &shader;

    return shader;
}
void UseShader(Shader shader)
{
    glUseProgram(shader.ID);
}
void SetShaderBool(int Shader_ID, const char *name, bool value)
{
    glUniform1i(glGetUniformLocation(Shader_ID, name), (int)value);
}
void SetShaderInt(int Shader_ID, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(Shader_ID, name), value);
}
void SetShaderFloat(int Shader_ID, const char *name, float value)
{
    glUniform1f(glGetUniformLocation(Shader_ID, name), value);
}
void SetShaderMat4(int Shader_ID, const char *name, mat4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(Shader_ID, name), 1, GL_FALSE, matrix);
}
void SetShaderVec2(int Shader_ID, const char *name, vec2 vector)
{
    glUniform2f(glGetUniformLocation(Shader_ID, name), vector[0], vector[1]);
}
void SetShaderVec3(int Shader_ID, const char *name, vec3 vector)
{
    glUniform3f(glGetUniformLocation(Shader_ID, name), vector[0], vector[1], vector[2]);
}

inline void SetShaderVec4(int Shader_ID, const char *name, vec4 value)
{
    glUniform4f(glGetUniformLocation(Shader_ID, name), value[0], value[1], value[2], value[3]);
}

void SetShaderVec3v(int Shader_ID, const char *name, vec3 *vector, int amount)
{
    glUniform3fv(glGetUniformLocation(Shader_ID, name), amount, vector);
}