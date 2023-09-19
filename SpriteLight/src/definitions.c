void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
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
        return false;
    }
    state->screen_width = new_width;
    state->screen_height = new_height;
    glm_ortho(0.0f, (float)new_width, 0.f, (float)new_height, state->near_z, state->far_z, state->ortho_projection);
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

void UpdateCamera()
{
    // camera stuff
    if (state->active_camera)
    {
        vec3 temp;
        glm_mat4_identity(state->view);
        vec3 camera_pos = {state->camera.position.x, state->camera.position.y, state->camera.position.z};
        glm_vec3_add(camera_pos, (vec3){0, 0, -1}, temp);
        glm_lookat(camera_pos, temp, (vec3){0, 1, 0}, state->view);
    }
}

Vector2 MeasureText(char *text, Font *font, float scale)
{
    scale *= 0.001f;
    float offset_x = 0;
    float max_y = 0;
    float xpos = 0;
    float w = 0;
    for (char i = 0; i != strlen(text); i++)
    {
        float h = 0;
        TextCharacter ch;
        if (font == NULL)
            ch = default_chars[text[i]];
        else
            ch = font->loaded_chars[text[i]];
        w = ch.size[0] * scale;
        h = ch.size[1] * scale;
        if (i == 0)
            offset_x -= ch.bearing[0] * scale;
        xpos = offset_x + ch.bearing[0] * scale;
        if (!max_y)
            max_y = h;
        if (h > max_y)
            max_y = h;
        offset_x += (ch.advance >> 6) * scale;
    }
    return (Vector2){xpos + w, max_y};
}

void EngineQuit(void)
{
    SDL_GL_DeleteContext(state->main_context);
    SDL_DestroyWindow(state->main_window);
    SDL_Quit();
}