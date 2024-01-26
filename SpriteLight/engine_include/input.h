#define MAX_INPUT_KEYS 512

typedef enum KeyCode {
    KEY_NONE,
    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
    KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
    KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
    KEY_SPACE, KEY_ENTER, KEY_ESCAPE, KEY_BACKSPACE, KEY_TAB,
    KEY_LEFT_SHIFT, KEY_RIGHT_SHIFT, KEY_LEFT_CTRL, KEY_RIGHT_CTRL, KEY_LEFT_ALT, KEY_RIGHT_ALT,
    KEY_LEFT_GUI, KEY_RIGHT_GUI,
    KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
    NUM_KEYS,
    MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_RIGHT,
    NUM_INPUTS,
    NUM_MOUSE_KEYS = 3
} KeyCode;

static const SDL_Scancode KeyCodeToScancode[NUM_KEYS] = {
    [KEY_A] = SDL_SCANCODE_A, [KEY_B] = SDL_SCANCODE_B, [KEY_C] = SDL_SCANCODE_C,
    [KEY_D] = SDL_SCANCODE_D, [KEY_E] = SDL_SCANCODE_E, [KEY_F] = SDL_SCANCODE_F,
    [KEY_G] = SDL_SCANCODE_G, [KEY_H] = SDL_SCANCODE_H, [KEY_I] = SDL_SCANCODE_I,
    [KEY_J] = SDL_SCANCODE_J, [KEY_K] = SDL_SCANCODE_K, [KEY_L] = SDL_SCANCODE_L,
    [KEY_M] = SDL_SCANCODE_M, [KEY_N] = SDL_SCANCODE_N, [KEY_O] = SDL_SCANCODE_O,
    [KEY_P] = SDL_SCANCODE_P, [KEY_Q] = SDL_SCANCODE_Q, [KEY_R] = SDL_SCANCODE_R,
    [KEY_S] = SDL_SCANCODE_S, [KEY_T] = SDL_SCANCODE_T, [KEY_U] = SDL_SCANCODE_U,
    [KEY_V] = SDL_SCANCODE_V, [KEY_W] = SDL_SCANCODE_W, [KEY_X] = SDL_SCANCODE_X,
    [KEY_Y] = SDL_SCANCODE_Y, [KEY_Z] = SDL_SCANCODE_Z, [KEY_0] = SDL_SCANCODE_0,
    [KEY_1] = SDL_SCANCODE_1, [KEY_2] = SDL_SCANCODE_2, [KEY_3] = SDL_SCANCODE_3,
    [KEY_4] = SDL_SCANCODE_4, [KEY_5] = SDL_SCANCODE_5, [KEY_6] = SDL_SCANCODE_6,
    [KEY_7] = SDL_SCANCODE_7, [KEY_8] = SDL_SCANCODE_8, [KEY_9] = SDL_SCANCODE_9,
    [KEY_SPACE] = SDL_SCANCODE_SPACE, [KEY_ENTER] = SDL_SCANCODE_RETURN,
    [KEY_ESCAPE] = SDL_SCANCODE_ESCAPE, [KEY_BACKSPACE] = SDL_SCANCODE_BACKSPACE,
    [KEY_TAB] = SDL_SCANCODE_TAB, [KEY_LEFT_SHIFT] = SDL_SCANCODE_LSHIFT,
    [KEY_RIGHT_SHIFT] = SDL_SCANCODE_RSHIFT, [KEY_LEFT_CTRL] = SDL_SCANCODE_LCTRL,
    [KEY_RIGHT_CTRL] = SDL_SCANCODE_RCTRL, [KEY_LEFT_ALT] = SDL_SCANCODE_LALT,
    [KEY_RIGHT_ALT] = SDL_SCANCODE_RALT, [KEY_LEFT_GUI] = SDL_SCANCODE_LGUI,
    [KEY_RIGHT_GUI] = SDL_SCANCODE_RGUI, [KEY_F1] = SDL_SCANCODE_F1,
    [KEY_F2] = SDL_SCANCODE_F2, [KEY_F3] = SDL_SCANCODE_F3, [KEY_F4] = SDL_SCANCODE_F4,
    [KEY_F5] = SDL_SCANCODE_F5, [KEY_F6] = SDL_SCANCODE_F6, [KEY_F7] = SDL_SCANCODE_F7,
    [KEY_F8] = SDL_SCANCODE_F8, [KEY_F9] = SDL_SCANCODE_F9, [KEY_F10] = SDL_SCANCODE_F10,
    [KEY_F11] = SDL_SCANCODE_F11, [KEY_F12] = SDL_SCANCODE_F12
};

static const KeyCode ScancodeToKeyCode[SDL_NUM_SCANCODES] = {
    [SDL_SCANCODE_A] = KEY_A, [SDL_SCANCODE_B] = KEY_B, [SDL_SCANCODE_C] = KEY_C,
    [SDL_SCANCODE_D] = KEY_D, [SDL_SCANCODE_E] = KEY_E, [SDL_SCANCODE_F] = KEY_F,
    [SDL_SCANCODE_G] = KEY_G, [SDL_SCANCODE_H] = KEY_H, [SDL_SCANCODE_I] = KEY_I,
    [SDL_SCANCODE_J] = KEY_J, [SDL_SCANCODE_K] = KEY_K, [SDL_SCANCODE_L] = KEY_L,
    [SDL_SCANCODE_M] = KEY_M, [SDL_SCANCODE_N] = KEY_N, [SDL_SCANCODE_O] = KEY_O,
    [SDL_SCANCODE_P] = KEY_P, [SDL_SCANCODE_Q] = KEY_Q, [SDL_SCANCODE_R] = KEY_R,
    [SDL_SCANCODE_S] = KEY_S, [SDL_SCANCODE_T] = KEY_T, [SDL_SCANCODE_U] = KEY_U,
    [SDL_SCANCODE_V] = KEY_V, [SDL_SCANCODE_W] = KEY_W, [SDL_SCANCODE_X] = KEY_X,
    [SDL_SCANCODE_Y] = KEY_Y, [SDL_SCANCODE_Z] = KEY_Z, [SDL_SCANCODE_0] = KEY_0,
    [SDL_SCANCODE_1] = KEY_1, [SDL_SCANCODE_2] = KEY_2, [SDL_SCANCODE_3] = KEY_3,
    [SDL_SCANCODE_4] = KEY_4, [SDL_SCANCODE_5] = KEY_5, [SDL_SCANCODE_6] = KEY_6,
    [SDL_SCANCODE_7] = KEY_7, [SDL_SCANCODE_8] = KEY_8, [SDL_SCANCODE_9] = KEY_9,
    [SDL_SCANCODE_SPACE] = KEY_SPACE, [SDL_SCANCODE_RETURN] = KEY_ENTER,
    [SDL_SCANCODE_ESCAPE] = KEY_ESCAPE, [SDL_SCANCODE_BACKSPACE] = KEY_BACKSPACE,
    [SDL_SCANCODE_TAB] = KEY_TAB, [SDL_SCANCODE_LSHIFT] = KEY_LEFT_SHIFT,
    [SDL_SCANCODE_RSHIFT] = KEY_RIGHT_SHIFT, [SDL_SCANCODE_LCTRL] = KEY_LEFT_CTRL,
    [SDL_SCANCODE_RCTRL] = KEY_RIGHT_CTRL, [SDL_SCANCODE_LALT] = KEY_LEFT_ALT,
    [SDL_SCANCODE_RALT] = KEY_RIGHT_ALT, [SDL_SCANCODE_LGUI] = KEY_LEFT_GUI,
    [SDL_SCANCODE_RGUI] = KEY_RIGHT_GUI, [SDL_SCANCODE_F1] = KEY_F1,
    [SDL_SCANCODE_F2] = KEY_F2, [SDL_SCANCODE_F3] = KEY_F3, [SDL_SCANCODE_F4] = KEY_F4,
    [SDL_SCANCODE_F5] = KEY_F5, [SDL_SCANCODE_F6] = KEY_F6, [SDL_SCANCODE_F7] = KEY_F7,
    [SDL_SCANCODE_F8] = KEY_F8, [SDL_SCANCODE_F9] = KEY_F9, [SDL_SCANCODE_F10] = KEY_F10,
    [SDL_SCANCODE_F11] = KEY_F11, [SDL_SCANCODE_F12] = KEY_F12
};

typedef enum InputState {
    INPUT_UP, INPUT_PRESS, INPUT_DOWN
} InputState;

typedef struct CallData
{
    void* data; int *count; int *byte_offsets;
}CallData;

typedef void (*InputActionCallback)(void *data);

typedef struct InputAction {
    KeyCode key;
    InputActionCallback callback;
    InputState trigger;
    char *name;
    void *data;
    struct InputAction *next;
} InputAction;

typedef struct {
    SDL_Scancode scancode;
    KeyCode keycode;
} KeyMap;

typedef struct Input {
    InputState key[MAX_INPUT_KEYS];
    InputAction *actions[NUM_INPUTS];
} Input;

typedef struct {
    size_t num_pointers;
    void **pointers;
} CallArgs;

CallArgs *ArgsToCallArgs(size_t count, ...);
void SetInputAction(KeyCode key, InputActionCallback callback, InputState key_state, char* name, CallArgs *data);
InputState *GetKeyState(KeyCode key);
bool GetInputPress(KeyCode key);
bool GetInputDown(KeyCode key);
unsigned int *GetAnyKey();
