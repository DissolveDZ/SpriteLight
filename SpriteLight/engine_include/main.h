#ifndef SPRITELIGHT_H
#define SPRITELIGHT_H

#include "shader.h"
#include "texture.h"
#include "lights.h"
#pragma once

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
    Texture texture;
    // Material mat;
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

typedef struct Entity
{
    float accel;
    float decel;
    Vector2 velocity;
    Vector2 pelocity;
    bool is_floor;
    float speed;
    float max_speed;
    float jump_height;
    float health;
    bool flip;
    Texture tex;
    Collider col;
    Rectangle floor_check;
} Entity;

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

typedef enum Player_State
{
    IDLE,
    WALKING
} Player_State;

typedef struct Player
{
    float turn_mult;
    float vertical_to_horizontal;
    Vector2 last_velocity;
    bool was_floor;
    Entity entity;
    Player_State state;
} Player;

typedef struct TextCharacter
{
    unsigned int texture_id; // ID handle of the glyph texture
    ivec2 size;              // Size of glyph
    ivec2 bearing;           // Offset from baseline to left/top of glyph
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
    Mix_Chunk *chunk;
} Sound;

typedef struct Music
{
    Mix_Music *music;
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

typedef struct State
{
    int screen_width;
    int screen_height;
    float near_z;
    float far_z;
    int target_fps;
    SDL_Event window_event;
    SDL_Window *main_window;
    SDL_GLContext main_context;

    Audio audio;

    Input input;

    Salad *salad;

    u8 *key_state;
    u32 mouse_state;
    Vector2 mouse_world;
    Vector2 camera_pan_start;
    Vector2 camera_pan_end;
    Vector2I mouse_pos;
    Vector2I mouse_delta;
    int wheel;
    bool deferred;
    bool sdf_font;
    void (*resize_callback)(int, int);
    Bloom bloom;
    Player player;
    Camera camera;
    double frame_time;
    float time;
    int active_camera;
    float gravity;
    bool quit;
    mat4 model, view, projection, ortho_projection;
    unsigned int max_colliders;
    unsigned int cur_colliders;
    bool fullscreen;
} State;

static Shader downsample_shader, upsample_shader, basic_shader, basic_screen_space_shader, circle_shader, text_shader_world, text_shader, gradient_shader;

static float line_vertices[] = { 0, 0, 0, 0, 0, 0 };

static float quad_vertices[] = {
    -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,
     1.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  0.0f,  1.0f,  0.0f
};

static float plane_vertices[] = {
    -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

static float cube_vertices[] = {
    // Front face
   -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // Vertex 0
    1.0f, -1.0f,  1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f, // Vertex 1
   -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f, // Vertex 2
    1.0f,  1.0f,  1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // Vertex 3

    // Right face
    1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Vertex 1
    1.0f, -1.0f, -1.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Vertex 5
    1.0f,  1.0f,  1.0f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f, // Vertex 3
    1.0f,  1.0f, -1.0f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, // Vertex 7

    // Back face
    1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f, // Vertex 5
   -1.0f, -1.0f, -1.0f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f, // Vertex 4
    1.0f,  1.0f, -1.0f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f, // Vertex 7
   -1.0f,  1.0f, -1.0f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f, // Vertex 6

   // Left face
   -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Vertex 4
   -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Vertex 0
   -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // Vertex 6
   -1.0f,  1.0f,  1.0f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // Vertex 2

    // Bottom face
   -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // Vertex 4
    1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // Vertex 5
   -1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // Vertex 0
    1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f, // Vertex 1

    // Top face
   -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Vertex 2
    1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Vertex 3
   -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Vertex 6
    1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Vertex 7
};
static State *state;

static int MAX_BLOOM_MIP = 10;
static u64 last_frame;
static u64 current_frame;

static unsigned int quad_vbo, quad_vao;
static unsigned int plane_vbo, plane_vao;
static unsigned int text_vbo, text_vao;
static unsigned int line_vbo, line_vao;
static unsigned int cube_vbo, cube_vao;

int GetRandomValue(int min, int max);
char* TextFormat(const char* format, ...);
float Lerp(float start, float end, float amount);

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
char *ReadTextFile(char *path);
// multiply vector2 by a value
Vector2 Vector2Scale(Vector2 vector, float scalar);
// add two vector2s
Vector2 Vector2Add(Vector2 first, Vector2 second);
Vector2 Vector2Zero(void);
Vector3 Vector3Scale(Vector3 vector, float scalar);
Vector3 Vector3Add(Vector3 first, Vector3 second);
Vector3 Vector3Subtract(Vector3 first, Vector3 second);
Vector3 Vector3Zero(void);
float Vector3Distance(Vector3 v1, Vector3 v2);
// comparing too floats might be innacurate, avoid doing this
bool Vector2Comp(Vector2 first, Vector2 second);
// return the delta of your mouse between frames
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
void LightingPass();

// initiate engine, higher mip level = more bloom samples and 0 is no bloom at all
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

#endif // SPRITELIGHT_H