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

void InitializeMappings() {
    #define MAP_KEY(key, code) \
        KeyHash[code].scancode = code; \
        KeyHash[code].keycode = key;

    KeyMap key_mappings[] = {
        {SDL_SCANCODE_A, KEY_A}, {SDL_SCANCODE_B, KEY_B}, {SDL_SCANCODE_C, KEY_C}, {SDL_SCANCODE_D, KEY_D},
        {SDL_SCANCODE_E, KEY_E}, {SDL_SCANCODE_F, KEY_F}, {SDL_SCANCODE_G, KEY_G}, {SDL_SCANCODE_H, KEY_H},
        {SDL_SCANCODE_I, KEY_I}, {SDL_SCANCODE_J, KEY_J}, {SDL_SCANCODE_K, KEY_K}, {SDL_SCANCODE_L, KEY_L},
        {SDL_SCANCODE_M, KEY_M}, {SDL_SCANCODE_N, KEY_N}, {SDL_SCANCODE_O, KEY_O}, {SDL_SCANCODE_P, KEY_P},
        {SDL_SCANCODE_Q, KEY_Q}, {SDL_SCANCODE_R, KEY_R}, {SDL_SCANCODE_S, KEY_S}, {SDL_SCANCODE_T, KEY_T},
        {SDL_SCANCODE_U, KEY_U}, {SDL_SCANCODE_V, KEY_V}, {SDL_SCANCODE_W, KEY_W}, {SDL_SCANCODE_X, KEY_X},
        {SDL_SCANCODE_Y, KEY_Y}, {SDL_SCANCODE_Z, KEY_Z}, {SDL_SCANCODE_0, KEY_0}, {SDL_SCANCODE_1, KEY_1},
        {SDL_SCANCODE_2, KEY_2}, {SDL_SCANCODE_3, KEY_3}, {SDL_SCANCODE_4, KEY_4}, {SDL_SCANCODE_5, KEY_5},
        {SDL_SCANCODE_6, KEY_6}, {SDL_SCANCODE_7, KEY_7}, {SDL_SCANCODE_8, KEY_8}, {SDL_SCANCODE_9, KEY_9},
        {SDL_SCANCODE_SPACE, KEY_SPACE}, {SDL_SCANCODE_RETURN, KEY_ENTER}, {SDL_SCANCODE_ESCAPE, KEY_ESCAPE},
        {SDL_SCANCODE_BACKSPACE, KEY_BACKSPACE}, {SDL_SCANCODE_TAB, KEY_TAB},
        {SDL_SCANCODE_LSHIFT, KEY_LEFT_SHIFT}, {SDL_SCANCODE_RSHIFT, KEY_RIGHT_SHIFT},
        {SDL_SCANCODE_LCTRL, KEY_LEFT_CTRL}, {SDL_SCANCODE_RCTRL, KEY_RIGHT_CTRL},
        {SDL_SCANCODE_LALT, KEY_LEFT_ALT}, {SDL_SCANCODE_RALT, KEY_RIGHT_ALT},
        {SDL_SCANCODE_LGUI, KEY_LEFT_GUI}, {SDL_SCANCODE_RGUI, KEY_RIGHT_GUI},
        {SDL_SCANCODE_F1, KEY_F1}, {SDL_SCANCODE_F2, KEY_F2}, {SDL_SCANCODE_F3, KEY_F3},
        {SDL_SCANCODE_F4, KEY_F4}, {SDL_SCANCODE_F5, KEY_F5}, {SDL_SCANCODE_F6, KEY_F6},
        {SDL_SCANCODE_F7, KEY_F7}, {SDL_SCANCODE_F8, KEY_F8}, {SDL_SCANCODE_F9, KEY_F9},
        {SDL_SCANCODE_F10, KEY_F10}, {SDL_SCANCODE_F11, KEY_F11}, {SDL_SCANCODE_F12, KEY_F12}
    };

    for (size_t i = 0; i < sizeof(key_mappings) / sizeof(key_mappings[0]); ++i) {
        MAP_KEY(key_mappings[i].keycode, key_mappings[i].scancode)
    }

    #undef MAP_KEY
}

KeyCode ScancodeToKeyCode(SDL_Scancode scancode) {
    return KeyHash[scancode].keycode;
}

SDL_Scancode KeyCodeToScancode(KeyCode keycode) {
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        if (KeyHash[i].keycode == keycode) {
            return KeyHash[i].scancode;
        }
    }
    return SDL_SCANCODE_UNKNOWN;
}

bool GetInputPress(KeyCode key)
{
    if (key > NUM_KEYS + NUM_MOUSE_KEYS+1)
        return false;
    return state->input.key[key] == INPUT_PRESS ? 1 : 0;
}

bool GetKeyDown(KeyCode key)
{
    if (key > NUM_KEYS + NUM_MOUSE_KEYS+1)
        return false;
    return state->key_state[KeyCodeToScancode(key)];
}

void SetInputAction(KeyCode key, InputActionCallback callback, InputState key_state)
{
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
    state->key_state = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&state->window_event))
    {
        state->mouse_state = SDL_GetMouseState(&state->mouse_pos.x, &state->mouse_pos.y);
        switch (state->window_event.type)
        {
        case SDL_QUIT:
            state->quit = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (state->window_event.button.button == SDL_BUTTON_RIGHT && state->camera.type == PANNING_CAMERA)
                state->camera_pan_start = GetScreenToWorld2D((Vector2){state->mouse_pos.x, state->mouse_pos.y}, state->projection);
            state->input.key[NUM_KEYS + state->window_event.button.button] = INPUT_PRESS;
            break;
        case SDL_MOUSEBUTTONUP:
            state->input.key[NUM_KEYS + state->window_event.button.button] = INPUT_UP;
            break;
        case SDL_KEYDOWN:
            if (!state->window_event.key.repeat)
                state->input.key[ScancodeToKeyCode(state->window_event.key.keysym.scancode)] = INPUT_PRESS;
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
                state->input.key[ScancodeToKeyCode(state->window_event.key.keysym.scancode)] = INPUT_UP;
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
        }
    }

    // Loop for keyboard inputs
    for (int i = 0; i < NUM_KEYS; i++)
    {
        InputAction *action = state->input.actions[i];

        if (action)
        {
            InputState trigger = state->input.key[i];
            if ((trigger == action->trigger) || (trigger == INPUT_PRESS && action->trigger == INPUT_DOWN))
                action->callback();
        }
        state->input.key[i] = state->key_state[KeyCodeToScancode(i)] ? INPUT_DOWN : INPUT_UP;
    }

    // Separate loop for mouse inputs
    for (int i = NUM_KEYS+1; i <= NUM_KEYS + NUM_MOUSE_KEYS; i++)
    {
        int mouse_index = i-NUM_KEYS;
        InputAction *action = state->input.actions[i];

        if (action)
        {
            InputState trigger = state->input.key[i];
            if ((trigger == action->trigger) || (trigger == INPUT_PRESS && action->trigger == INPUT_DOWN))
                action->callback();
        }
        state->input.key[i] = (state->mouse_state & SDL_BUTTON(mouse_index)) ? INPUT_DOWN : INPUT_UP;
    }

}