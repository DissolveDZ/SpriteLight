static u32 max_input_keys = 512;
typedef struct Input
{
    char pressed[max_input_keys];
}Input;

/*
typedef enum KeyState
{
    KEY_PRESSED,
    KEY_DOWN,
    KEY_UP
}KeyState;
*/

typedef struct KeyState
{
    bool pressed;
    bool down;
    bool up;
}KeyState;

KeyState GetKeyState(char key);