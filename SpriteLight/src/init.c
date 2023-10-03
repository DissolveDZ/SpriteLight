State *EngineInit(char *window_name, char *icon_path, int width, int height, int bloom_mip_level)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    state = calloc(1, sizeof(State));
    state->main_window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    state->main_context = SDL_GL_CreateContext(state->main_window);
    InitAudio();
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    SDL_GL_SetSwapInterval(1);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // make shader loading save already loaded shaders into an array to avoid loading multiple
    basic_shader = LoadShader("engine/rec.vert", "engine/rec.frag");
    basic_screen_space_shader = LoadShader("ui/screen_space.vert", "engine/rec.frag");
    text_shader = LoadShader("engine/text.vert", "engine/text.frag");
    text_shader_world = LoadShader("engine/text_world.vert", "engine/text.frag");
    gradient_shader = LoadShader("engine/quad.vert", "ui/gradient.frag");

    SDL_Surface *icon = LoadSDLImage(icon_path);
    SDL_SetWindowIcon(state->main_window, icon);
    stbi_set_flip_vertically_on_load(1);

    state->time = 0;
    state->near_z = 0.1f;
    state->far_z = 1000.f;
    state->fullscreen = false;
    state->screen_width = width;
    state->screen_height = height;
    state->quit = false;
    state->resize_callback = 0;
    current_frame = SDL_GetPerformanceCounter();
    OnResize(width, height);

    InitDefaultFont(512);
    BufferSetup(&quad_vao, &quad_vbo, quad_vertices, sizeof(quad_vertices), true, false);
    BufferSetup(&text_vao, &text_vbo, quad_vertices, sizeof(quad_vertices), true, false);
    BufferSetup(&plane_vao, &plane_vbo, plane_vertices, sizeof(plane_vertices), true, false);
    if (bloom_mip_level)
        BloomInit(bloom_mip_level);
    return state;
}

bool IsFontLoaded(Font *font, char *path)
{
    if (loaded_fonts_len >= loaded_fonts_max - 1)
    {
        loaded_fonts_max *= 1.5f;
        loaded_fonts = realloc(loaded_fonts, loaded_fonts_max * sizeof(Font));
    }
    else if (!loaded_fonts_len)
    {
        loaded_fonts = calloc(1, loaded_fonts_max * sizeof(Font));
        return false;
    }
    for (int i = 0; i < loaded_fonts_len; i++)
    {
        if (strcmp(loaded_fonts[i]->path, path) == 0)
        {
            printf("already loaded\n");
            font = loaded_fonts[i];
            return true;
        }
    }
    return false;
}

void InitDefaultFont(unsigned int resolution)
{
    Font *default_font = LoadFont("resources/fonts/arial.ttf", resolution);
    memcpy(default_chars, default_font->loaded_chars, 128 * sizeof(TextCharacter));
}

Font *LoadFont(char *path, unsigned int resolution)
{
    Font *font;
    if (IsFontLoaded(font, path))
        return font;
    if (!strlen(path))
    {
        printf("please enter a valid path");
        return NULL;
    }
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        printf("Could not init FreeType Library");
        return NULL;
    }
    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face))
    {
        printf("Failed to load font");
        return NULL;
    }
    else
    {
        font = calloc(1, sizeof(Font));
        font->path = path;
        FT_Set_Pixel_Sizes(face, 0, resolution);

        // disable byte-alignment restriction
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
        loaded_fonts[loaded_fonts_len] = font;
        loaded_fonts_len++;
        return font;
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}