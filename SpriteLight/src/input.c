// depricated for now
/*
KeyState *GetKeyState(KeyCode key)
{
    if (!key || key > NUM_KEYS)
        return NULL;
    key = KeyMapping[key].scancode;
    if (state->key_state[key])
        state->input.key[key].down = true;
    return &state->input.key[key];
}
*/

bool GetKeyPress(KeyCode key)
{
    if (!key || key > NUM_KEYS)
        return false;
    return state->input.key[key] == KEY_PRESS ? 1 : 0;
}

void SetKeyAction(KeyCode key, KeyActionCallback callback)
{
    if (!key || key > NUM_KEYS)
        return;
    KeyAction *new_action = state->input.actions[key];
    if (new_action)
        new_action->callback = callback;
    else
    {
        new_action = malloc(sizeof(KeyAction));
        new_action->key = key;
        new_action->callback = callback;
        new_action->next = NULL;
    }
    state->input.actions[key] = new_action;
}

void ProcessActionEvent()
{
    for (int i = 0; i < NUM_KEYS; i++)
    {
        KeyAction *action = state->input.actions[i];
        if (action)
        {
            KeyCode key = action->key;
            SDL_Scancode scancode = KeyMapping[key].scancode;
            if (state->key_state[scancode])
                action->callback();
        }
    }
}

void ProcessKeys()
{
    SDL_Scancode cur_code;
    while (SDL_PollEvent(&state->window_event))
    {
        ProcessKeys();
        switch (state->window_event.type)
        {
        case SDL_QUIT:
            state->quit = true;
            break;
        case SDL_KEYDOWN:
            cur_code = state->window_event.key.keysym.scancode;
            if (cur_code && state->window_event.key.repeat == 0)
            {
                state->input.key[cur_code].pressed = true;
                ProcessActionEvent();
            }
            switch (state->window_event.key.keysym.scancode)
            case SDL_SCANCODE_F11:
                state->fullscreen = !state->fullscreen;
            if (state->fullscreen)
                SDL_SetWindowFullscreen(state->main_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            else
                SDL_SetWindowFullscreen(state->main_window, SDL_WINDOW_BORDERLESS);
            break;
            break;
        case SDL_KEYUP:
            cur_code = state->window_event.key.keysym.scancode;
            if (cur_code && state->window_event.key.repeat == 0)
            {
                state->input.key[cur_code].up = true;
                ProcessActionEvent();
            }
        case SDL_WINDOWEVENT:
            if (state->window_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                if (state->window_event.window.data1 || state->window_event.window.data2)
                    OnResize(state->window_event.window.data1, state->window_event.window.data2);
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
}