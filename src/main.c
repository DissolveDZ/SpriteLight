#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <win_include.h>
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "container.h"

int main(void)
{
    State *state = EngineInit("engine test", "resources/textures/cube.png", 1920, 1080, 0);
    // optionally enable opengl debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    Camera *camera = CreateCamera2D(45.f, (Vector3){0, 0, 25}, PANNING_CAMERA);
    Rectangle rec = (Rectangle){0, 2, 1, 1};
    rec.texture = LoadTexture2D("resources/textures/cube.png", 0, false);
    Font *pixel_square = LoadFont("resources/fonts/Pixel_Square.ttf", 512);
    Font *antonio_bold = LoadFont("resources/fonts/Antonio-Bold.ttf", 512);
    Text text = (Text){"TESTING default font()", 5.f, -5.f, 2.5f, {255, 0, 0, 255}};
    while (!state->quit)
    {
        UpdateKeys();
        EngineUpdate();
        UpdateCamera();
        glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawRectangle((Rectangle){0, 0, 2, 1}, (Vector4){125, 125, 125, 255});
        DrawRectangle((Rectangle){-3, 0, 2, 1}, (Vector4){255, 75, 75, 255});
        DrawRectangle((Rectangle){3, 0, 2, 1}, (Vector4){75, 75, 75, 255});
        DrawRectangleTex(rec);
        Vector3 text_size = MeasureTextText(&text, 0);
        DrawRectangle((Rectangle){text.x + text_size.x / 2, text.y + text_size.y / 2 + text_size.z, text_size.x, text_size.y}, (Vector4){0, 0, 0, 255});
        DrawTextWorldText(text, 0);
        DrawTextWorld("this is a test", pixel_square, -7, 0, 2.5f, (Vector4){0, 0, 255, 255});
        Vector3 text_dimensions = MeasureText("Measuring Text..()", pixel_square, 1.5f);
        DrawRectangle((Rectangle){state->mouse_world.x + text_dimensions.x / 2, state->mouse_world.y + text_dimensions.y / 2 + text_dimensions.z, text_dimensions.x, text_dimensions.y}, (Vector4){0, 0, 0, 255});
        DrawTextWorld("Measuring Text..()", pixel_square, state->mouse_world.x, state->mouse_world.y, 1.5f, (Vector4){255, 0, 0, 125});
        DrawSubText("100% health", pixel_square, round((sinf(state->time)*0.5f+0.5f)*strlen("100% health")), 0.f, 0.f, 0.125f, (Vector4){255, 0, 0, 255});
        SDL_GL_SwapWindow(state->main_window);
    }
    EngineQuit();
    return 0;
}