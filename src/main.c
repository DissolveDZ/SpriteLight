#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <cglm/call.h>
#include <cglm/struct.h>
#include "win_include.h"
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "container.h"

void TestAction()
{
    printf("press\n");
}

void TestAction1()
{
    printf("down\n");
}

void TestAction2()
{
    if (GetInputPress(MOUSE_LEFT))
    {
        printf("Mouse Left Pressed\n");
    }
    if (GetInputPress(MOUSE_RIGHT))
    {
        printf("Mouse Right Held\n");
    }
}

int main(void)
{
    State *state = EngineInit("engine test", "resources/textures/cube.png", 1920, 1080, 6);
    u32 music = LoadAudioStream("resources/audio/music/35_Return_Trip.mp3");
    u32 sound1 = LoadSound("resources/audio/sounds/cash.mp3");
    u32 sound2 = LoadSound("resources/audio/sounds/water.mp3");
    SetAudioStreamVolume(music, 10);
    SetVolume(sound1, 1000);
    SetVolume(sound2, 1000);
    // PlayAudioStream(music);
    //  optionally enable opengl debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    Camera *camera = CreateCamera2D(45.f, (Vector3){0, 0, 25}, PANNING_CAMERA);
    Rectangle rec = (Rectangle){0, 2, 1, 1};
    rec.texture = LoadTexture2D("resources/textures/cube.png", 0, false);
    Font *pixel_square = LoadFont("resources/fonts/Pixel_Square.ttf", 512);
    Font *antonio_bold = LoadFont("resources/fonts/Antonio-Bold.ttf", 512);
    Text text = (Text){"TESTING default font", 5.f, -5.f, 2.5f, {255, 0, 0, 255}};
    SetInputAction(KEY_A, TestAction, INPUT_PRESS);
    SetInputAction(KEY_T, TestAction1, INPUT_DOWN);
    SetInputAction(MOUSE_LEFT, TestAction2, INPUT_PRESS);
    //SetInputAction(MOUSE_LEFT, TestAction2, INPUT_PRESS);
    SetInputAction(MOUSE_RIGHT, TestAction2, INPUT_PRESS);

    while (!state->quit)
    {
        EngineUpdate();
        if (GetKeyDown(KEY_D))
        {
            camera->position.x += 15.f * state->frame_time;
        }
        if (GetKeyDown(KEY_A))
        {
            camera->position.x -= 15.f * state->frame_time;
        }
        if (GetKeyDown(KEY_W))
        {
            camera->position.y += 15.f * state->frame_time;
        }
        if (GetKeyDown(KEY_S))
        {
            camera->position.y -= 15.f * state->frame_time;
        }
        UpdateCamera();
        glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawGradientV((Vector4){51, 76.5f, 153, 255}, (Vector4){178.5f, 229.5f, 255, 255}, state->camera.position.y / 100);
        DrawRect((Rectangle){0, 0, 2, 1}, (Vector4){125, 125, 125, 255});
        DrawRect((Rectangle){-3, 0, 2, 1}, (Vector4){255, 75, 75, 255});
        DrawRect((Rectangle){3, 0, 2, 1}, (Vector4){75, 75, 75, 255});
        DrawTexRect(rec);
        Vector3 text_size = MeasureWorldTextText(&text, 0);
        DrawRect((Rectangle){text.x + text_size.x / 2, text.y + text_size.y / 2 + text_size.z, text_size.x, text_size.y}, (Vector4){25, 25, 25, 50});
        DrawWorldTextText(text, 0);
        DrawWorldText("this is a test", pixel_square, -7, 0, 2.5f, (Vector4){0, 0, 255, 255});
        text_size = MeasureWorldText("Measuring Text..()", antonio_bold, 1.5f);
        DrawRect((Rectangle){state->mouse_world.x + text_size.x / 2, state->mouse_world.y + text_size.y / 2 + text_size.z, text_size.x, text_size.y}, (Vector4){25, 25, 25, 50});
        DrawWorldText("Measuring Text..()", antonio_bold, state->mouse_world.x, state->mouse_world.y, 1.5f, (Vector4){255, 0, 0, 255});
        text_size = MeasureText("100% he\nalth", pixel_square, 0.125f);
        DrawUIRect((Rectangle){25 + text_size.x / 2, 125.f - text_size.y / 2 + text_size.z, text_size.x, text_size.y}, (Vector4){125, 125, 125, 50});
        DrawSubText("100% he\na-l\nth", pixel_square, round((sinf(state->time) * 0.5f + 0.5f) * strlen("100% he\na-l\nth")), 25.f, 125.f, 0.125f, (Vector4){255, 0, 0, 255});
        // DrawUIRect((Rectangle){0, 0, state->screen_width, state->screen_height}, (Vector4){255, 255, 255, 255});
        SDL_GL_SwapWindow(state->main_window);
    }
    EngineQuit();
    return 0;
}