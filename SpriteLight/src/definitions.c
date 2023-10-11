char *ReadTextFile(char *path)
{
    void *file;
    long fsize;
    file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    fsize = ftell(file);
    rewind(file);
    path = malloc(fsize + 1);
    fread(path, 1, fsize, file);
    path[fsize] = 0;
    return path;
}

char *FormatShaderUniform(const char *uniform_name, int index)
{
    int buffer_size = snprintf(NULL, 0, "%s[%d].%s", uniform_name, index, uniform_name) + 1;
    char *buffer = malloc(buffer_size);
    snprintf(buffer, buffer_size, "%s[%d].%s", uniform_name, index, uniform_name);
    return buffer;
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}

char *TextFormat(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    int length = vsnprintf(NULL, 0, format, args); // Determine the length of the formatted string
    if (length < 0)
    {
        // Handle error
        va_end(args);
        return NULL;
    }

    char *result = (char *)malloc(length + 1); // Allocate memory for the formatted string (+1 for null terminator)
    if (result == NULL)
    {
        // Handle memory allocation failure
        va_end(args);
        return NULL;
    }

    va_end(args); // Reset the va_list

    va_start(args, format);                      // Start again for the actual formatting
    vsnprintf(result, length + 1, format, args); // Format the string and copy it to the result buffer
    va_end(args);

    return result;
}

int GetRandomValue(int min, int max)
{
    if (min > max)
    {
        int tmp = max;
        max = min;
        min = tmp;
    }
    if ((unsigned int)(max - min) > (unsigned int)RAND_MAX)
    {
        printf("value exceeds %i", RAND_MAX);
    }
    return (rand() % (abs(max - min) + 1) + min);
}

float Lerp(float start, float end, float amount)
{
    float result = start + amount * (end - start);

    return result;
}

Camera *CreateCamera2D(float fov, Vector3 position, CameraType type)
{
    state->active_camera++;
    state->camera.fov = fov;
    state->camera.position = position;
    state->camera.zoom = state->camera.position.z;
    state->camera.target = Vector3Zero();
    state->camera.type = type;
    return &state->camera;
}

void OnResize(int new_width, int new_height)
{
    if (new_width > (int)INT_MAX || new_height > (int)INT_MAX)
    {
        printf("Window size overflow");
        return;
    }
    if (state->resize_callback)
        state->resize_callback(new_width, new_height);
    state->screen_width = new_width;
    state->screen_height = new_height;
    glm_ortho(0.0f, (float)new_width, (float)new_height, 0.f, -1.f, 1.f, state->ortho_projection);
    glViewport(0, 0, new_width, new_height);
}

Vector3 Vector3Transform(vec3 v, mat4 mat)
{
    Vector3 result;
    float x = v[0];
    float y = v[1];
    float z = v[2];
    result.x = mat[0][0] * x + mat[1][0] * y + mat[2][0] * z + mat[3][0];
    result.y = mat[0][1] * x + mat[1][1] * y + mat[2][1] * z + mat[3][1];
    result.z = mat[0][2] * x + mat[1][2] * y + mat[2][2] * z + mat[3][2];
    return result;
}

Vector2 GetScreenToWorld2D(Vector2 position, mat4 projection)
{
    Vector2 clipcoord = {2.0f * position.x / (float)state->screen_width - 1.0f, 1.0 - 2.0 * position.y / (float)state->screen_height};
    mat4 inv_view;
    glm_mat4_inv(projection, inv_view);
    Vector3 transform;
    // glm_scale(invmat, transform);
    transform = Vector3Transform((vec3){clipcoord.x, clipcoord.y, 1}, inv_view);
    // transform = (vec4){position.x, position.y, 1, 1} * inv_view;
    return (Vector2){transform.x * state->camera.position.z + state->camera.position.x, transform.y * state->camera.position.z + state->camera.position.y};
}

Vector2 Vector2Scale(Vector2 vector, float scalar)
{
    return (Vector2){vector.x * scalar, vector.y * scalar};
}

Vector2 Vector2Add(Vector2 first, Vector2 second)
{
    return (Vector2){first.x + second.x, first.y + second.y};
}

Vector3 Vector3Scale(Vector3 vector, float scalar)
{
    return (Vector3){vector.x * scalar, vector.y * scalar, vector.z * scalar};
}

Vector3 Vector3Add(Vector3 first, Vector3 second)
{
    return (Vector3){first.x + second.x, first.y + second.y, first.z + second.z};
}

Vector3 Vector3Subtract(Vector3 first, Vector3 second)
{
    return (Vector3){first.x - second.x, first.y - second.y, first.z - second.z};
}

Vector2 Vector2Zero()
{
    return (Vector2){0};
}

Vector3 Vector3Zero()
{
    return (Vector3){0};
}

// return the distance between two vector3s
float Vector3Distance(Vector3 first, Vector3 second)
{
    float dirx = second.x - first.x;
    float diry = second.y - first.y;
    float dirz = second.z - first.z;
    return sqrtf(dirx * dirx + diry * diry + dirz * dirz);
}

bool Vector2Comp(Vector2 first, Vector2 second)
{
    return (first.x == second.x && first.y == second.y);
}

void CameraZoom(Camera *camera, float amount, float min, float max)
{
    float distance = Vector3Distance(camera->position, camera->target);

    distance += amount;

    if (distance < min)
        distance = min;
    else if (distance > max)
        distance = max;

    Vector3 forward = (Vector3){0, 0, -1};
    camera->zoom = distance;
    Vector3 next = Vector3Add(camera->target, Vector3Scale(forward, -distance));

    camera->zoom = next.z;
    camera->position = next;
}

Vector3 MeasureWorldTextText(Text *text, Font *font)
{
    return MeasureWorldText(text->text, font, text->scale);
}

Vector3 MeasureTextText(Text *text, Font *font)
{
    return MeasureText(text->text, font, text->scale);
}
Vector3 MeasureText(char *text, Font *font, float scale)
{
    if (text == NULL || text[0] == '\0')
    {
        return (Vector3){0.0f, 0.0f, 0.0f}; // Empty text, return zeros.
    }

    bool first_row = true;
    float offset_x = 0.0f;
    float offset_y = 0.0f;
    float offset_y_min = 0.0f;
    float offset_x_max = 0.0f;
    float max_y = 0.0f;
    float highest_height = 0.0f;
    float first_offset = 0.0f;

    for (int i = 0; text[i] != '\0'; i++)
    {
        char current_char = text[i];
        TextCharacter ch;

        if (!font)
            ch = default_chars[current_char];
        else
            ch = font->loaded_chars[current_char];

        float w = ch.size[0] * scale;
        float h = ch.size[1] * scale;

        if (h > highest_height)
            highest_height = h;

        if (text[i] == '\n')
        {
            offset_x = 0;
            offset_y += highest_height;
            if (first_row)
                first_offset = highest_height;
            highest_height = 0;
            first_row = false;
            // continue; for no spacing
        }

        if (i == 0)
        {
            offset_x -= ch.bearing[0] * scale;
        }

        float xpos = offset_x + ch.bearing[0] * scale;
        float ypos = offset_y - (ch.size[1] - ch.bearing[1]) * scale;

        if (ypos < offset_y_min)
            offset_y_min = ypos;

        if (xpos + w > offset_x_max)
            offset_x_max = xpos + w;

        float temp = h + ypos;

        if (temp > max_y)
            max_y = temp;

        offset_x += (ch.advance >> 6) * scale;
    }
    return (Vector3){offset_x_max, (max_y - offset_y_min), offset_y_min + first_offset};
}

Vector3 MeasureWorldText(char *text, Font *font, float scale)
{
    return MeasureText(text, font, scale * 0.001f);
}

void EnginePresent(void)
{
    SDL_GL_SwapWindow(state->main_window);
    last_frame = current_frame;
    current_frame = SDL_GetPerformanceCounter();
    state->frame_time = (current_frame - last_frame) / (double)SDL_GetPerformanceFrequency();
    state->time += state->frame_time;
    double target_frame_time = 1000.0 / state->target_fps; // Target frame time for 60 FPS in milliseconds
    double frame_delay = target_frame_time - state->frame_time;
    if (frame_delay > 0) {
        SDL_Delay(frame_delay);
    }
    /*
    frame_delay = 1.0 / state->target_fps - state->frame_time;
    if (frame_delay > 0)
    {
        Uint32 delay_time = (Uint32)(frame_delay*1000);
        SDL_Delay(delay_time);
    }
    */
}

void EngineQuit(void)
{
    FreeResources();
    SDL_GL_DeleteContext(state->main_context);
    SDL_DestroyWindow(state->main_window);
    SDL_Quit();
}