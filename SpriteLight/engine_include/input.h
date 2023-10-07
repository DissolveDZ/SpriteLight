const static unsigned int max_input_keys = 512;

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

typedef enum InputState {
    INPUT_UP, INPUT_PRESS, INPUT_DOWN
} InputState;

typedef void (*InputActionCallback)(void);

typedef struct InputAction {
    KeyCode key;
    InputActionCallback callback;
    InputState trigger;
    struct InputAction *next;
} InputAction;

typedef struct {
    SDL_Scancode scancode;
    KeyCode keycode;
} KeyMap;

static KeyMap KeyHash[SDL_NUM_SCANCODES];

void InitializeMappings();

// Function to get the KeyCode from a SDL_Scancode
KeyCode ScancodeToKeyCode(SDL_Scancode scancode);

// Function to get the SDL_Scancode from a KeyCode
SDL_Scancode KeyCodeToScancode(KeyCode keycode);

typedef struct Input {
    InputState key[max_input_keys];
    InputAction *actions[NUM_INPUTS];
} Input;

void SetInputAction(KeyCode key, InputActionCallback callback, InputState key_state);
InputState *GetKeyState(KeyCode key);
bool GetInputPress(KeyCode key);
bool GetKeyDown(KeyCode key);
unsigned int *GetAnyKey();