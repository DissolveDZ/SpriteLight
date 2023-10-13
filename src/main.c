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
    state = EngineInit("engine test", "resources/textures/cube.png", 1920, 1080, 0);
    Texture tex1 = LoadTexture("cube.png");
    Texture tex2 = LoadTexture("vedl.png");
    u32 music = LoadAudioStream("resources/audio/music/crisp_5.flac");
    u32 sound1 = LoadSound("resources/audio/sounds/cash.mp3");
    u32 sound2 = LoadSound("resources/audio/sounds/water.mp3");
    SetAudioStreamVolume(music, 10);
    SetVolume(sound1, 1000);
    SetVolume(sound2, 1000);
    // PlayAudioStream(music);
    // glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    Camera *camera = CreateCamera2D(45.f, (Vector3){0, 0, 25}, PANNING_CAMERA);
    Rectangle rec = (Rectangle){0, 2, 1, 1};
    //rec.texture = LoadTexture("cube.png");
    Font *pixel_square = LoadFont("Pixel_Square.ttf", 256);
    Font *antonio_bold = LoadFont("Antonio-Bold.ttf", 256);
    Text text = (Text){"TESTING default font", 5.f, -5.f, 5.f, {255, 0, 0, 255}};
    float camera_speed = 5.f;
    // pass in a pointer to your arguments
    SetInputAction(KEY_A, MoveLeft, INPUT_DOWN, "Left", ArgsToCallArgs(2, camera, &camera_speed));
    SetInputAction(KEY_D, MoveRight, INPUT_DOWN, "Right", ArgsToCallArgs(2, camera, &camera_speed));
    SetInputAction(KEY_W, MoveUp, INPUT_DOWN, "Up", ArgsToCallArgs(2, camera, &camera_speed));
    SetInputAction(KEY_S, MoveDown, INPUT_DOWN, "Down", ArgsToCallArgs(2, camera, &camera_speed));
    while (!state->quit)
    {
        EngineUpdate();
        UpdateCamera();
        glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        BeginBatch();
        DrawRect((Rectangle){0, 0, 2, 1}, tex1, -state->time * 200);
        for (int y = 0; y < 50; y++)
        {
            for (int x = 0; x < 50; x++)
            {
                GLuint tex_id = (x + y) % 2 == 0 ? tex1.ID : tex2.ID;
                Texture tex;
                tex.ID = tex_id;
                DrawRect((Rectangle){x, y, 1, 1}, tex, 0.f);
            }
        }
        EndBatch();
        FlushBatch();
        EnginePresent();
    }
    EngineQuit();
    return 0;
}