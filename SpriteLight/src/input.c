#include "engine.h"

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

bool GetInputPress(KeyCode key)
{
    if (key > NUM_KEYS + NUM_MOUSE_KEYS + 1)
        return false;
    return state->input.key[key] == INPUT_PRESS ? 1 : 0;
}

bool GetInputDown(KeyCode key)
{
    if (key > NUM_KEYS + NUM_MOUSE_KEYS + 1)
        return false;
    if (key < NUM_KEYS)
        return state->key_state[KeyCodeToScancode[key]];
    else
        return state->mouse_state & SDL_BUTTON(key - NUM_KEYS);
}

CallArgs *ArgsToCallArgs(size_t count, ...)
{
    va_list list;
    va_start(list, count);

    CallArgs *args = malloc(sizeof(CallArgs));
    args->pointers = (void **)malloc(count * sizeof(void *));
    args->num_pointers = count;

    for (int i = 0; i < count; i++)
    {
        args->pointers[i] = va_arg(list, void *);
    }
    va_end(list);
    return args;
}

void SetInputAction(KeyCode key, InputActionCallback callback, InputState key_state, char *name, CallArgs *args)
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
    new_action->name = name;
    new_action->data = args;
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
        case SDL_EVENT_QUIT:
            state->quit = true;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (state->window_event.button.button == SDL_BUTTON_RIGHT && state->camera.type == PANNING_CAMERA)
                state->camera_pan_start = GetScreenToWorld2D((Vector2){state->mouse_pos.x, state->mouse_pos.y}, state->projection);
            state->input.key[NUM_KEYS + state->window_event.button.button] = INPUT_PRESS;
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            state->input.key[NUM_KEYS + state->window_event.button.button] = INPUT_UP;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (!state->window_event.key.repeat)
                state->input.key[ScancodeToKeyCode[state->window_event.key.keysym.scancode]] = INPUT_PRESS;
        case SDL_EVENT_KEY_UP:
            if (!state->window_event.key.repeat)
                state->input.key[ScancodeToKeyCode[state->window_event.key.keysym.scancode]] = INPUT_UP;
            break;
        case SDL_EVENT_WINDOW_RESIZED:
                 if (state->window_event.window.data1 || state->window_event.window.data2)
                    OnResize(state->window_event.window.data1, state->window_event.window.data2);
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            state->wheel = state->window_event.wheel.y;
            break;
        }
    }

    // Loop for keyboard inputs
    for (int i = 0; i < NUM_KEYS; i++)
    {
        InputAction *action = state->input.actions[i];

        if (action && action->data && action->callback)
        {
            InputState trigger = state->input.key[i];
            if ((trigger == action->trigger) || (trigger == INPUT_PRESS && action->trigger == INPUT_DOWN))
                action->callback(action->data);
        }
        state->input.key[i] = state->key_state[KeyCodeToScancode[i]] ? INPUT_DOWN : INPUT_UP;
    }

    // Separate loop for mouse inputs
    for (int i = NUM_KEYS + 1; i <= NUM_KEYS + NUM_MOUSE_KEYS; i++)
    {
        int mouse_index = i - NUM_KEYS;
        InputAction *action = state->input.actions[i];

        if (action && action->data && action->callback)
        {
            InputState trigger = state->input.key[i];
            if ((trigger == action->trigger) || (trigger == INPUT_PRESS && action->trigger == INPUT_DOWN))
                action->callback(action->data);
        }
        state->input.key[i] = (state->mouse_state & SDL_BUTTON(mouse_index)) ? INPUT_DOWN : INPUT_UP;
    }
}
