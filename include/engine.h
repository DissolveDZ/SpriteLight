# 0 "SpriteLight/engine_include/SpriteLight.h"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "SpriteLight/engine_include/SpriteLight.h"
# 1 "SpriteLight/engine_include/input.h" 1


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
    InputState key[512];
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
# 2 "SpriteLight/engine_include/SpriteLight.h" 2
# 1 "SpriteLight/engine_include/main.h" 1



# 1 "SpriteLight/engine_include/shader.h" 1
       

char *ReadTextFile(char *path);



static char* FormatShaderUniform(const char* uniform_name, int index);

typedef struct Shader
{
    unsigned int ID;
} Shader;

Shader LoadShader(const char *vertex_name, const char *fragment_name);

void UseShader(Shader shader);

void SetShaderBool(int Shader_ID, const char *name, bool value);

void SetShaderInt(int Shader_ID, const char *name, int value);

void SetShaderFloat(int Shader_ID, const char *name, float value);

void SetShaderMat4(int Shader_ID, const char *name, mat4 matrix);

void SetShaderVec2(int Shader_ID, const char *name, vec2 vector);

void SetShaderVec3(int Shader_ID, const char *name, vec3 vector);

void SetShaderVec4(int Shader_ID, const char *name, vec4 value);

void SetShaderVec3v(int Shader_ID, const char *name, vec3 *vector, int amount);
# 5 "SpriteLight/engine_include/main.h" 2
# 1 "SpriteLight/engine_include/texture.h" 1
       

typedef struct Texture
{
    unsigned int ID;
    char *path;
    char *type;
    int width;
    int height;
} Texture;

Texture LoadTexture(const char *path);
SDL_Surface *LoadSDLImage(char *path);
# 6 "SpriteLight/engine_include/main.h" 2
# 1 "SpriteLight/engine_include/lights.h" 1
       


static int MAX_POINT_LIGHTS = 10;

typedef struct PointLight
{
    vec3 position;
    float padding1;
    vec3 color;
    float padding2;
    vec3 ambient;
    float intensity;
    float radius;
    vec3 padding3;
} PointLight;

typedef struct DirectionalLight
{
    vec3 color;
    vec3 ambient;
    vec3 direction;
} DirectionalLight;

typedef struct SpotLight
{
    vec3 pos;
    vec3 color;
    vec3 ambient;
    vec3 direction;
    float cutoff;
    float outer_cutoff;
    float falloff_mulitplier;
} SpotLight;

static int point_light_count;

static PointLight *point_lights;
static GLuint lights_ubo;
static GLuint lights_index = 0;
static GLuint lights_binding_point = 0;

void InitLights();

PointLight *CreatePointLight(vec3 position, vec3 color, vec3 ambient, float intensity, float radius);

void UpdateLights();

void FreeLights();
# 7 "SpriteLight/engine_include/main.h" 2
       

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef vec2s Vector2;
typedef ivec2s Vector2I;
typedef vec3s Vector3;
typedef ivec3s Vector3I;
typedef vec4s Vector4;
typedef ivec4s Vector4I;

typedef enum CameraType
{
    DEFAULT_CAMERA,
    PANNING_CAMERA
} CameraType;

typedef struct Camera
{
    Vector3 position;
    Vector3 target;
    CameraType type;
    float angle;
    float fov;
    float zoom;
} Camera;

typedef struct
{
    Vector3 position;
    Vector2 tex_coords;
    float tex_id;
}Vertex;


typedef struct Material
{

} Material;

typedef struct Line
{
    Vector2 start;
    Vector2 end;
} Line;

typedef struct Rectangle
{
    float x;
    float y;
    float width;
    float height;
} Rectangle;

typedef struct Collider
{
    float x;
    float y;
    float width;
    float height;
    Line *vertices;
    bool dynamic;
    bool rotating;
} Collider;

typedef struct BloomMip
{
    vec2 size;
    ivec2 int_size;
    Texture texture;
} BloomMip;
typedef struct Bloom
{
    unsigned int mip_chain_len;
    BloomMip *mip_chain;
    unsigned int FBO;
    bool karis_average;
    bool enabled;
} Bloom;

typedef struct PointIntersect
{
    Vector2 dist;
    bool hit;
} PointIntersect;

typedef struct TextCharacter
{
    unsigned int texture_id;
    ivec2 size;
    ivec2 bearing;
    unsigned int advance;
} TextCharacter;
static TextCharacter default_chars[128];

typedef struct Font
{
    char *path;
    TextCharacter loaded_chars[128];
} Font;

typedef struct Text
{
    char *text;
    float x;
    float y;
    float scale;
    Vector4 color;
} Text;

static unsigned int text_characters_max = 100;

typedef struct Sound
{

 u32 volume;
} Sound;

typedef struct Music
{

    u32 volume;
} Music;

typedef struct Audio
{
    Music **music;
    u32 music_len;
    u32 music_max;
    Sound **sounds;
    u32 parallel_sounds;
    u32 max_parallel_sounds;
    u32 max_parallel_musics;
    u32 sounds_playing;
    u32 sounds_len;
    u32 sounds_max;
    u32 volume;
} Audio;

typedef struct {
    char file_path[256];
    int index;
    void* data;
    u32 program_index;
} Resource;

typedef struct HashNode {
    Resource resource;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode **table;
    size_t size;
    size_t capacity;
} HashTable;

typedef struct {
    HashTable *hash_table;
    int next_index;
} Salad;

typedef struct {
    GLuint count;
    GLuint prim_count;
    GLuint first;
    GLuint base_instance;
} DrawCommand;

typedef struct {
 GLuint command_buffer;

    GLuint white;
    u32 white_ID;

    u32 batch_count;
 u32 current_shader;
 u32 current_batch;

    u32 *textures;
    u32 tex_count;

    u32 max_quads;
    u32 max_vertices;
    u32 max_textures;
} Renderer;

typedef struct State
{
    u32 screen_width;
    u32 screen_height;
    float near_z;
    float far_z;
    u32 target_fps;
    SDL_Event window_event;
    SDL_Window *main_window;
    SDL_GLContext main_context;

    Audio audio;

    Input input;

    Salad *salad;

    Renderer renderer;

    u8 *key_state;
    u32 mouse_state;
    Vector2 mouse_world;
    Vector2 camera_pan_start;
    Vector2 camera_pan_end;
    Vector2 mouse_pos;
    Vector2 mouse_delta;
    int wheel;
    bool deferred;
    bool sdf_font;
    void (*resize_callback)(int, int);
    Bloom bloom;
    Camera camera;
    f64 frame_time;
    f64 time;
    u32 active_camera;
    f32 gravity;
    bool quit;
    mat4 model, view, projection, ortho_projection;
    u32 max_colliders;
    u32 cur_colliders;
    bool fullscreen;
} State;

static Shader downsample_shader, upsample_shader, general_shader, ui_shader, text_shader, gradient_shader, circle_shader, text_shader_world, text_shader;
static float line_vertices[6];

static float quad_vertices[] = {
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f
};

static float plane_vertices[] = {
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};

static float cube_vertices[] = {

   -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
   -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,


    1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,


    1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
   -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
   -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,


   -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
   -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
   -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
   -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,


   -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
   -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,


   -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
   -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
};
static State *state;

static int MAX_BLOOM_MIP = 10;
static u64 last_frame;
static u64 current_frame;

static u32 quad_vbo, quad_vao;
static u32 plane_vbo, plane_vao;
static u32 text_vbo, text_vao;
static u32 line_vbo, line_vao;
static u32 cube_vbo, cube_vao;

int GetRandomValue(int min, int max);
char* TextFormat(const char* format, ...);
float Lerp(float start, float end, float amount);

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
char *ReadTextFile(char *path);

Vector2 Vector2Scale(Vector2 vector, float scalar);

Vector2 Vector2Add(Vector2 first, Vector2 second);
Vector2 Vector2Zero(void);
Vector3 Vector3Scale(Vector3 vector, float scalar);
Vector3 Vector3Add(Vector3 first, Vector3 second);
Vector2 Vector2Subtract(Vector2 first, Vector2 second);
Vector3 Vector3Subtract(Vector3 first, Vector3 second);
Vector3 Vector3Zero(void);
Vector3 Vector2ToVector3(Vector2 vec, float z);
float Vector3Distance(Vector3 v1, Vector3 v2);
Vector3 Vector3Normalize(Vector3 vector);

bool Vector2Comp(Vector2 first, Vector2 second);

Vector2 GetMouseDelta(void);
Vector2 GetMouseWorldDelta(void);
Vector3 Vector3Transform(vec3 v, mat4 mat);
Vector2 GetScreenToWorld2D(Vector2 position, mat4 projection);
Camera *CreateCamera2D(float fov, Vector3 position, CameraType type);
void GBufferSetup(unsigned int *g_buffer, unsigned int *g_position, unsigned int *g_normal, unsigned int *g_albedo, unsigned int *depth, int screen_width, int screen_height);
void PostProcessBuffer(unsigned int *post_process_fbo, unsigned int *post_process_color, unsigned int *depth, int screen_width, int screen_height);
void BufferSetup(unsigned int *VAO, unsigned int *VBO, float vertices[], int size, bool textured, bool normals);
void OnResize(int new_width, int new_height);
Font *LoadFont(const char *font_name, unsigned int resolution);
void InitDefaultFont(unsigned int resolution);
Vector3 MeasureText(char *text, Font *font, float scale);
Vector3 MeasureTextText(Text *text, Font *font);
Vector3 MeasureWorldText(char *text, Font *font, float scale);
Vector3 MeasureWorldTextText(Text *text, Font *font);
void CameraZoom(Camera *camera, float amount, float min, float max);
void ZoomCamera(Camera *Camera);
void LightingPass();


State *EngineInit(char *window_name, char *icon_path, int width, int height, int bloom_mip_level);
void EngineUpdate();
void EnginePresent(void);
void UpdateKeys();
void UpdateCamera();
void EngineQuit(void);
void BloomInit(int mip_amount);
void UpsampleBloom(float filter_radius);
void DownSampleBloom(unsigned int src_texture, float threshold, float knee);
void RenderBloom(unsigned int src_texture, float filter_radius, float threshold, float knee);
# 3 "SpriteLight/engine_include/SpriteLight.h" 2


# 1 "SpriteLight/engine_include/resource_mgr.h" 1



void InitHashTable(size_t initial_capacity);
void FreeResources();
Resource *LoadResource(const char* file_name);
Resource *GetResource(int index_or_file_path);
void RemoveResource(int index_or_file_path);
Texture LoadTexture(const char *texture_name);
Shader Load_Shader(const char *vertex_name, const char *fragment_name);
unsigned int Hash(const char *str);
void HandleError(const char *message, ...);
# 6 "SpriteLight/engine_include/SpriteLight.h" 2

# 1 "SpriteLight/engine_include/draw.h" 1
void DrawQuad();
void DrawRect(Rectangle rec, Vector4 color);
void DrawUIRect(Rectangle rec, Vector4 color);
void DrawTexRect(Rectangle rec, u32 tex_id, float rotation);
void DrawTexRectTint(Rectangle rec, u32 tex_id, float rotation, Vector4 tint);
void DrawWorldText(char *text, Font *font, float x, float y, float scale, Vector4 color);
void DrawWorldTextText(Text text, Font *font);
void DrawText(char *text, Font *font, float x, float y, float scale, Vector4 color);
void DrawTextText(Text text, Font *font);
void DrawSubText(char *text, Font *font, int count, float x, float y, float scale, Vector4 color);
void DrawSubTextText(Text *text, Font *font, int count);
void DrawCube(Vector3 position, Vector3 scale, Vector3 rotation, Texture texture);
void DrawGradientV(Vector4 start, Vector4 end, float offset);
void DrawLine2DWorld(Vector2 start, Vector2 end, Vector4 color);
# 8 "SpriteLight/engine_include/SpriteLight.h" 2
