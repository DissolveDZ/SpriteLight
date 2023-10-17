State *EngineInit(char *window_name, char *icon_path, int width, int height, int bloom_mip_level)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    state = calloc(1, sizeof(State));
    state->main_window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    state->main_context = SDL_GL_CreateContext(state->main_window);
    InitAudio();
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    SDL_GL_SetSwapInterval(1);
    SDL_DisplayMode cur_mode;
    if (SDL_GetCurrentDisplayMode(0, &cur_mode) != 0)
    {
        HandleError("Failed to get display mode! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }
    state->target_fps = cur_mode.refresh_rate;

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LESS);

    InitHashTable(16);
    // InitDefaultFont(256);

    basic_shader = LoadShader("engine/quad.vert", "engine/quad.frag");
    gradient_shader = LoadShader("engine/quad.vert", "engine/test.frag");
    // gradient_shader = LoadShader("engine/quad.vert", "ui/gradient.frag");

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
    BatchSetup();
    if (bloom_mip_level)
        BloomInit(bloom_mip_level);
    return state;
}

void InitDefaultFont(unsigned int resolution)
{
    Font *default_font = LoadFont("arial.ttf", resolution);
    memcpy(default_chars, default_font->loaded_chars, 128 * sizeof(TextCharacter));
}

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
            else
            {
                font = calloc(1, sizeof(Font));
                font->path = font_path;
                FT_Set_Pixel_Sizes(face, 0, resolution);

                // Disable byte-alignment restriction
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

                for (unsigned char c = 0; c < 128; c++)
                {
                    // Load character glyph
                    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                    {
                        printf("Failed to load Glyph");
                        continue;
                    }

                    if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LIGHT))
                        continue;

                    if (state->sdf_font)
                        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_SDF);

                    unsigned int texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);
                    glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_RED,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        0,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                    TextCharacter character =
                        {texture,
                         {face->glyph->bitmap.width, face->glyph->bitmap.rows},
                         {face->glyph->bitmap_left, face->glyph->bitmap_top},
                         {face->glyph->advance.x}};
                    font->loaded_chars[c] = character;
                }

                resource->data = font;
                glBindTexture(GL_TEXTURE_2D, 0);
                FT_Done_Face(face);
                FT_Done_FreeType(ft);
                return font;
            }
        }
    }
    return NULL;
}