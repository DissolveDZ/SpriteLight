void ProcessCamera(Camera *camera)
{
    switch (camera->type)
    {
    default:
        break;
    case PANNING_CAMERA:
        if (state->mouse_state & SDL_BUTTON_RMASK)
        {
            state->camera_pan_end = GetScreenToWorld2D((Vector2){state->mouse_pos.x, state->mouse_pos.y}, state->projection);
            camera->position.x += (state->camera_pan_start.x - state->camera_pan_end.x);
            camera->position.y += (state->camera_pan_start.y - state->camera_pan_end.y);
            state->camera_pan_start = GetScreenToWorld2D((Vector2){state->mouse_pos.x, state->mouse_pos.y}, state->projection);
        }
        if (state->wheel != 0)
        {
            camera->target.x = state->mouse_world.x;
            camera->target.y = state->mouse_world.y;
            Vector2 before = (Vector2){camera->target.x, camera->target.y};
            CameraZoom(&state->camera, -1.f * state->wheel, 1.f, 100.f);
            Vector2 after = GetScreenToWorld2D((Vector2){state->mouse_pos.x, state->mouse_pos.y}, state->projection);
            camera->position.x += (before.x - after.x);
            camera->position.y += (before.y - after.y);
        }
        break;
    }
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

void EngineUpdate()
{
    state->wheel = 0;
    memset(state->input.key, 0, max_input_keys * sizeof(KeyState));
    last_frame = current_frame;
    current_frame = SDL_GetPerformanceCounter();
    state->frame_time = (current_frame - last_frame) / (double)SDL_GetPerformanceFrequency();
    state->time += state->frame_time;
    state->key_state = SDL_GetKeyboardState(NULL);
    SDL_GetRelativeMouseState(&state->mouse_delta.x, &state->mouse_delta.y);
    glm_mat4_identity(state->projection);
    if (state->screen_height && state->screen_width)
        glm_perspective(glm_rad((float)state->screen_height / state->camera.fov), (float)state->screen_width / (float)state->screen_height, state->near_z, state->far_z, state->projection);
    state->mouse_state = SDL_GetMouseState(&state->mouse_pos.x, &state->mouse_pos.y);
    state->mouse_world = GetScreenToWorld2D((Vector2){state->mouse_pos.x, state->mouse_pos.y}, state->projection);
    ProcessCamera(&state->camera);
}