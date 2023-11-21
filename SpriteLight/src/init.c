#include "engine.h"
#include "msdfgl.h"

State *EngineInit(char *window_name, char *icon_path, int width, int height, int bloom_mip_level)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	state = calloc(1, sizeof(State));
	state->main_window = SDL_CreateWindow(window_name, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	state->main_context = SDL_GL_CreateContext(state->main_window);
	SDL_SetWindowPosition(state->main_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	// InitAudio();
	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	SDL_GL_SetSwapInterval(1);
	const SDL_DisplayMode *cur_mode = SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());
	if (!cur_mode)
	{
		HandleError("Failed to get display mode! SDL_Error: %s", SDL_GetError());
		SDL_Quit();
		return NULL;
	}
	state->target_fps = cur_mode->refresh_rate;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_Surface *icon = LoadSDLImage(icon_path);
	SDL_SetWindowIcon(state->main_window, icon);
	stbi_set_flip_vertically_on_load(1);
	current_frame = SDL_GetPerformanceCounter();
	state->frame_time = 0;
	state->time = 0;
	state->near_z = 0.1f;
	state->far_z = 1000.f;
	state->fullscreen = false;
	state->screen_width = width;
	state->screen_height = height;
	state->quit = false;
	state->resize_callback = 0;
	OnResize(width, height);

	InitHashTable(16);
	InitDefaultFont(256);
	
	general_shader = LoadShader("engine/rec.vert", "engine/rec.frag");
	ui_shader = LoadShader("ui/screen_space.vert", "engine/rec.frag");
	sdf_text_shader = LoadShader("engine/sdf_text.vert", "engine/sdf_text.frag");
	text_shader = LoadShader("engine/text.vert", "engine/text.frag");
	text_shader_world = LoadShader("engine/text_world.vert", "engine/text.frag");
	gradient_shader = LoadShader("engine/quad.vert", "ui/gradient.frag");

	SetInputAction(KEY_F11, ToggleFullscreen, INPUT_PRESS, "Toggle Fullscreen", 0);
	// SetInputAction(MOUSE_RIGHT, CameraPan, INPUT_PRESS, "Camera Pan", 0);


	BufferSetup(&quad_vao, &quad_vbo, quad_vertices, sizeof(quad_vertices), true, false);
	BufferSetup(&text_vao, &text_vbo, quad_vertices, sizeof(quad_vertices), true, false);
	BufferSetup(&plane_vao, &plane_vbo, plane_vertices, sizeof(plane_vertices), true, false);
	BufferSetup(&line_vao, &line_vbo, line_vertices, sizeof(line_vertices), false, false);
	BufferSetup(&cube_vao, &cube_vbo, cube_vertices, sizeof(cube_vertices), true, true);

	if (bloom_mip_level)
		BloomInit(bloom_mip_level);
	return state;
}

void InitDefaultFont(unsigned int resolution)
{
	state->msdfgl_context = msdfgl_create_context("460 core");
	state->atlas = msdfgl_create_atlas(state->msdfgl_context, 1024, 2);
	/* Enable auto-generating undefined glyphs as they are encountered for the first time. */
	msdfgl_set_missing_glyph_callback(state->msdfgl_context, msdfgl_generate_glyph, NULL);
}

#define NUM_GLYPHS 127
// unnest code, will use freetype-GL anyways
Font *LoadFont(const char *font_name, unsigned int resolution)
{
	const char *base_path = "resources/fonts/";
	size_t font_path_length = strlen(base_path) + strlen(font_name) + 1;
	char *font_path = (char *)malloc(font_path_length);

	if (!font_path)
	{
		HandleError("Failed to allocate memory for font");
		return NULL;
	}

	snprintf(font_path, font_path_length, "%s%s", base_path, font_name);

	Resource *resource = LoadResource(font_path);
	Font *font;

	if (resource)
	{
		if (resource->data)
		{
			font = (Font *)resource->data;
			return font;
		}
		else
		{
			FT_Library ft;

			if (FT_Init_FreeType(&ft))
			{
				printf("Could not initialize FreeType Library");
				free(font_path);
				return NULL;
			}

			FT_Face face;

			if (FT_New_Face(ft, font_path, 0, &face))
			{
				printf("Failed to load font");
				free(font_path);
				FT_Done_FreeType(ft);
				return NULL;
			}
			font = calloc(1, sizeof(Font));
			font->path = font_path;
			resolution = 512;
			//f32 char_height = resolution << 6;
			//f32 char_width = char_height;
			FT_Set_Char_Size(face, 0, 16 << 6, resolution, resolution);
			
			// Disable byte-alignment restriction
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// Estimate the texture dimensions
			int max_dim = (1 + (face->size->metrics.height >> 6)) * ceilf(sqrtf(NUM_GLYPHS));
			int tex_width = 1;
			while (tex_width < max_dim)
				tex_width <<= 1;
			int tex_height = tex_width;

			// Allocate memory for pixel data
			char *pixels = (char *)calloc(tex_width * tex_height, 1);
			int pen_x = 0, pen_y = 0;

			for (unsigned char i = 32; i < NUM_GLYPHS; i++)
			{
				FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
				FT_Bitmap *bmp = &face->glyph->bitmap;
				if (!bmp)
					continue;
				if (pen_x + bmp->width >= tex_width)
				{
					pen_x = 0;
					pen_y += ((face->size->metrics.height >> 6) + 1);
				}

				for (int row = 0; row < bmp->rows; ++row)
				{
					for (int col = 0; col < bmp->width; ++col)
					{
						int x = pen_x + col;
						int y = tex_height - (pen_y + row) - 1; // Flip Y-coordinate
						pixels[y * tex_width + x] = bmp->buffer[row * bmp->pitch + col];
					}
				}

				// Initialize the information for individual glyphs

				font->characters[i].size.x = bmp->width;
				font->characters[i].size.y = bmp->rows;
				font->characters[i].bearing.x = pen_x;
				font->characters[i].bearing.y = -pen_y;
				font->characters[i].advance = face->glyph->advance.x >> 6;
				pen_x += bmp->width + 1;
			}

			u32 texture_atlas;
			glGenTextures(1, &texture_atlas);
			glBindTexture(GL_TEXTURE_2D, texture_atlas);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				tex_width,
				tex_height,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			font->texture_atlas.id = texture_atlas;
			font->texture_atlas.num_columns = 1;
			font->texture_atlas.resolution = tex_width;

			resource->data = font;
			glBindTexture(GL_TEXTURE_2D, 0);
			FT_Done_Face(face);
			FT_Done_FreeType(ft);
			free(pixels);
			return font;
		}
	}
	return NULL;
}
