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
            CameraZoom(&state->camera, -2.5f * state->wheel, 1.f, 100.f);
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
    last_frame = current_frame;
    current_frame = SDL_GetPerformanceCounter();
    state->frame_time = (double)(current_frame - last_frame) / (double)SDL_GetPerformanceFrequency();
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

void UpdateKeys()
{
    switch (state->window_event.type)
    {
    case SDL_QUIT:
        state->quit = true;
        break;
    case SDL_KEYDOWN:
        switch (state->window_event.key.keysym.scancode)
        case SDL_SCANCODE_F11:
            state->fullscreen = !state->fullscreen;
        if (state->fullscreen)
            SDL_SetWindowFullscreen(state->main_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        else
            SDL_SetWindowFullscreen(state->main_window, SDL_WINDOW_BORDERLESS);
        break;
        break;
    case SDL_WINDOWEVENT:
        if (state->window_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            if (state->window_event.window.data1 || state->window_event.window.data2)
                state->resize_ptr(state->window_event.window.data1, state->window_event.window.data2);
        }
        break;
    case SDL_MOUSEWHEEL:
        state->wheel = state->window_event.wheel.y;
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (state->window_event.button.button == SDL_BUTTON_RIGHT && state->camera.type == PANNING_CAMERA)
            state->camera_pan_start = GetScreenToWorld2D((Vector2){state->mouse_pos.x, state->mouse_pos.y}, state->projection);
        break;
    }
}