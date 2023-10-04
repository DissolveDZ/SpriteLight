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
    return state->input.key[KeyMapping[key].scancode] == INPUT_PRESS ? 1 : 0;
}

bool GetKeyDown(KeyCode key)
{
    if (!key || key > NUM_KEYS)
        return false;
    return state->key_state[KeyMapping[key].scancode];
}

void SetInputAction(KeyCode key, InputActionCallback callback, InputState key_state)
{
    if (!key || key > NUM_KEYS)
        return;
    InputAction *new_action = state->input.actions[key];
    if (new_action)
    {
        new_action->callback = callback;
        new_action->trigger = key_state;
    }
    else
    {
        new_action = malloc(sizeof(InputAction));
        new_action->key = key;
        new_action->callback = callback;
        new_action->trigger = key_state;
        new_action->next = NULL;
    }
    state->input.actions[key] = new_action;
}

void ProcessKeys()
{
    SDL_Scancode cur_code = 0;
    state->key_state = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&state->window_event))
    {
        switch (state->window_event.type)
        {
        case SDL_QUIT:
            state->quit = true;
            break;
        case SDL_KEYDOWN:
            if (!state->window_event.key.repeat)
            {
                cur_code = state->window_event.key.keysym.scancode;
                state->input.key[cur_code] = INPUT_PRESS;
            }
            switch (state->window_event.key.keysym.scancode)
            {
            case SDL_SCANCODE_F11:
                state->fullscreen = !state->fullscreen;
                if (state->fullscreen)
                    SDL_SetWindowFullscreen(state->main_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                else
                    SDL_SetWindowFullscreen(state->main_window, 0); // Set to 0 for windowed mode
                break;
            }
            break;
        case SDL_KEYUP:
            if (!state->window_event.key.repeat)
            {
                cur_code = state->window_event.key.keysym.scancode;
                state->input.key[cur_code] = INPUT_UP;
            }
            break;
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
            cur_code = state->window_event.button.button;
            if (cur_code >= 1 && cur_code <= NUM_MOUSE_CODES)
            {
                state->input.key[NUM_KEYS + cur_code] = INPUT_PRESS;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            cur_code = state->window_event.button.button;
            if (cur_code >= 1 && cur_code <= NUM_MOUSE_CODES)
            {
                state->input.key[NUM_KEYS + cur_code] = INPUT_UP;
            }
            break;
        }
    }
    for (int i = 0; i < NUM_KEYS + NUM_MOUSE_CODES; i++)
    {
        SDL_Scancode scancode = KeyMapping[i].scancode;
        InputAction *action = state->input.actions[i];
        if (action)
        {
            InputState trigger = state->input.key[scancode];

            // Check if the trigger matches the action's trigger
            if ((trigger == action->trigger) || (trigger == INPUT_PRESS && action->trigger == INPUT_HELD))
                action->callback();
        }
        state->input.key[scancode] = state->key_state[scancode] ? INPUT_HELD : INPUT_UP;
    }
}