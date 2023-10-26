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
#include <flecs.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "container.h"

void MoveLeft(void *data)
{
    CallArgs *args = (CallArgs *)data;
    Camera *camera = (Camera *)args->pointers[0];
    float speed = *(float *)args->pointers[1];
    if (camera)
        camera->position.x -= speed * state->frame_time;
}
void MoveRight(void *data)
{
    CallArgs *args = (CallArgs *)data;
    Camera *camera = (Camera *)args->pointers[0];
    float speed = *(float *)args->pointers[1];
    if (camera)
        camera->position.x += speed * state->frame_time;
}

void MoveUp(void *data)
{
    CallArgs *args = (CallArgs *)data;
    Camera *camera = (Camera *)args->pointers[0];
    float speed = *(float *)args->pointers[1];
    if (camera)
        camera->position.y += speed * state->frame_time;
}

void MoveDown(void *data)
{
    CallArgs *args = (CallArgs *)data;
    Camera *camera = (Camera *)args->pointers[0];
    float speed = *(float *)args->pointers[1];
    if (camera)
        camera->position.y -= speed * state->frame_time;
}

void Talk()
{
    printf("ain't no way\n");
}

void TestAction2()
{
    if (GetInputDown(MOUSE_LEFT))
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
    state = EngineInit("engine test", "resources/textures/cube.png", 1920, 1080, 6);
    u32 music = LoadAudioStream("resources/audio/music/crisp_5.flac");
    u32 sound1 = LoadSound("resources/audio/sounds/cash.mp3");
    u32 sound2 = LoadSound("resources/audio/sounds/water.mp3");
    SetAudioStreamVolume(music, 10);
    SetVolume(sound1, 1000);
    SetVolume(sound2, 1000);
    // PlayAudioStream(music);
    //   optionally enable opengl debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    Camera *camera = CreateCamera2D(45.f, (Vector3){0, 0, 25}, PANNING_CAMERA);
    Rectangle rec = (Rectangle){0, 2, 1, 1};
    LoadTexture("cube.png");
    rec.texture = LoadTexture("cube.png");
    Font *pixel_square = LoadFont("Pixel_Square.ttf", 256);
    Font *antonio_bold = LoadFont("Antonio-Bold.ttf", 256);
    Text text = (Text){"TESTING default font", 5.f, -5.f, 5.f, {255, 0, 0, 255}};
    float camera_speed = 10.f;
    // pass in a pointer to your arguments
    SetInputAction(KEY_A, MoveLeft, INPUT_DOWN, "Left", ArgsToCallArgs(2, camera, &camera_speed));
    SetInputAction(KEY_D, MoveRight, INPUT_DOWN, "Right", ArgsToCallArgs(2, camera, &camera_speed));
    SetInputAction(KEY_W, MoveUp, INPUT_DOWN, "Up", ArgsToCallArgs(2, camera, &camera_speed));
    SetInputAction(KEY_S, MoveDown, INPUT_DOWN, "Down", ArgsToCallArgs(2, camera, &camera_speed));
    SetInputAction(KEY_T, Talk, INPUT_PRESS, "Talk", 0);
    SetInputAction(MOUSE_LEFT, TestAction2, INPUT_DOWN, "LeftClick", 0);
    SetInputAction(MOUSE_RIGHT, TestAction2, INPUT_PRESS, "Interact", 0);
    Texture cube = LoadTexture("cube.png");
    while (!state->quit)
    {
        EngineUpdate();
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
        DrawWorldText("this is a test", pixel_square, -7, 0, 5.f, (Vector4){0, 0, 255, 255});
        text_size = MeasureWorldText("Measuring Text..()", antonio_bold, 3.f);
        DrawRect((Rectangle){state->mouse_world.x + text_size.x / 2, state->mouse_world.y + text_size.y / 2 + text_size.z, text_size.x, text_size.y}, (Vector4){25, 25, 25, 50});
        DrawWorldText("Measuring Text..()", antonio_bold, state->mouse_world.x, state->mouse_world.y, 3.f, (Vector4){255, 0, 0, 255});
        text_size = MeasureText("100% he\nal\nth", pixel_square, 0.25f);
        DrawUIRect((Rectangle){25 + text_size.x / 2, 125.f + text_size.y / 2 - text_size.z, text_size.x, text_size.y}, (Vector4){125, 125, 125, 50});
        DrawSubText("100% he\na-l\nth", pixel_square, round((sinf(state->time) * 0.5f + 0.5f) * strlen("100% he\na-l\nth")), 25.f, 125.f, 0.25f, (Vector4){255, 0, 0, 255});
        DrawLine2DWorld((Vector2){0, 0}, state->mouse_world, (Vector4){255, 0, 0, 255});
        DrawCube((Vector3){1, 1.5, 0.5f}, (Vector3){1, 1, 1}, (Vector3){20, 0, 20}, cube);
        DrawCube((Vector3){5, 2, 0.5f}, (Vector3){5, 1, 1}, (Vector3){40, 40.f, 20.f}, cube);
        // DrawUIRect((Rectangle){0, 0, state->screen_width, state->screen_height}, (Vector4){255, 255, 255, 255});
        EnginePresent();
    }
    EngineQuit();
    return 0;
}