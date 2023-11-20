#ifndef SPRITELIGHT_H
#define SPRITELIGHT_H

#include "msdfgl.h"
#define MAX_BLOOM_MIP 10

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
} Vertex;

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
	Vector2I size;	  // Size of glyph
	Vector2I bearing; // Offset from baseline to left/top of glyph
	unsigned int advance;
} TextCharacter;

typedef struct
{
	unsigned int id;
	int num_columns;
	int character_width;
	int character_height;
	int resolution;
} TextureAtlas;

typedef struct Font
{
	char *path;
	TextureAtlas texture_atlas;
	TextCharacter characters[128];
} Font;

typedef struct Text
{
	char *text;
	float x;
	float y;
	float scale;
	Vector4 color;
} Text;

typedef struct Sound
{
	// Mix_Chunk *chunk;
	u32 volume;
} Sound;

typedef struct Music
{
	// Mix_Music *music;
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

typedef struct
{
	char file_path[256];
	int index;
	void *data;
	u32 program_index;
} Resource;

typedef struct HashNode
{
	Resource resource;
	struct HashNode *next;
} HashNode;

typedef struct
{
	HashNode **table;
	size_t size;
	size_t capacity;
} HashTable;

typedef struct
{
	HashTable *hash_table;
	int next_index;
} Salad;

typedef struct
{
	GLuint count;
	GLuint prim_count;
	GLuint first;
	GLuint base_instance;
} DrawCommand;

typedef struct
{
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
	msdfgl_context_t msdfgl_context;
	msdfgl_atlas_t atlas;
	GLfloat msdf_ortho[4][4];

	Audio audio;

	Input input;

	Salad *salad;

	Renderer renderer;

	const u8 *key_state;
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

extern unsigned int text_characters_max;

extern Shader downsample_shader, upsample_shader, general_shader, ui_shader, text_shader, gradient_shader, circle_shader, text_shader_world, text_shader, sdf_text_shader;
extern float line_vertices[6];

extern float quad_vertices[20];

extern float plane_vertices[20];
extern float cube_vertices[192];
extern State *state;

extern u64 last_frame;
extern u64 current_frame;

extern u32 quad_vbo, quad_vao;
extern u32 plane_vbo, plane_vao;
extern u32 text_vbo, text_vao;
extern u32 line_vbo, line_vao;
extern u32 cube_vbo, cube_vao;

void ToggleFullscreen();

char *TextFormat(const char *format, ...);
TextCharacter CalculateCharacterInfo(TextureAtlas atlas, char character);

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
char *ReadTextFile(char *path);

// MATH

// RGB to Hex
i32 RGBToHex(Vector4I color);
// returns a random value between min and max
int GetRandomValue(int min, int max);
// interpolates between two floats
float Lerp(float start, float end, float amount);
// multiply Vector2 by a value
Vector2 Vector2Scale(Vector2 vector, float scalar);
// add two Vector2s
Vector2 Vector2Add(Vector2 first, Vector2 second);
// returns an empty Vector2
Vector2 Vector2Zero(void);
// Subtract a Vector2 by a Vector2
Vector2 Vector2Subtract(Vector2 first, Vector2 second);
// "scale" a Vector, which is multiplying the vector by a given number
Vector3 Vector3Scale(Vector3 vector, float scalar);
// Add two Vector3s
Vector3 Vector3Add(Vector3 first, Vector3 second);
// Subtract a Vector3 by a Vector3
Vector3 Vector3Subtract(Vector3 first, Vector3 second);
// returns an empty Vector3
Vector3 Vector3Zero(void);
// easy Vector2 to Vector3 conversion 
Vector3 Vector2ToVector3(Vector2 vec, float z);
// distance between two 3D Vectors
float Vector3Distance(Vector3 v1, Vector3 v2);
// normalize a 3D Vector
Vector3 Vector3Normalize(Vector3 vector);
// comparing two floats might be innacurate, avoid doing this
bool Vector2Comp(Vector2 first, Vector2 second);
// depricated, use cglm function
Vector3 Vector3Transform(vec3 v, mat4 mat);

// return the delta of your mouse between frames
Vector2 GetMouseDelta(void);
Vector2 GetMouseWorldDelta(void);
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

// initiate engine, higher mip level = more bloom samples and 0 is no bloom at all
State *EngineInit(char *window_name, char *icon_path, int width, int height, int bloom_mip_level);
void EngineUpdate();
void EnginePresent(void);
void UpdateKeys();
void CameraPan();
void UpdateCamera();
void EngineQuit(void);
void BloomInit(int mip_amount);
void UpsampleBloom(float filter_radius);
void DownSampleBloom(unsigned int src_texture, float threshold, float knee);
void RenderBloom(unsigned int src_texture, float filter_radius, float threshold, float knee);

#endif // SPRITELIGHT_H
