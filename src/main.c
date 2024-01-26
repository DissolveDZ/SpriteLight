#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <stdint.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <cglm/call.h>
#include <cglm/struct.h>
#include <flecs.h>
#include "win_include.h"
#include <glad/glad.h>
#include <msdfgl.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "engine.h"

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

int main(int argc, char *argv[])
{
	state = EngineInit("engine test", "resources/textures/cube.png", 1920, 1080, 0);
	Texture tex1 = LoadTexture("cube.png");
	Texture tex2 = LoadTexture("vedl.png");

	// glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
	Camera *camera = CreateCamera2D(45.f, (Vector3){0, 0, 25}, PANNING_CAMERA);
	Rectangle rec = (Rectangle){0, 2, 1, 1};

	//Font *pixel_square = LoadFont("Pixel_Square.ttf", 256);
	//Font *antonio_bold = LoadFont("Antonio-Bold.ttf", 256);
	Text text = (Text){"TESTING default font", 5.f, -5.f, 5.f, {255, 0, 0, 255}};
	float camera_speed = 5.f;

	// pass in a pointer to your arguments
	SetInputAction(KEY_A, MoveLeft, INPUT_DOWN, "Left", ArgsToCallArgs(2, camera, &camera_speed));
	SetInputAction(KEY_D, MoveRight, INPUT_DOWN, "Right", ArgsToCallArgs(2, camera, &camera_speed));
	SetInputAction(KEY_W, MoveUp, INPUT_DOWN, "Up", ArgsToCallArgs(2, camera, &camera_speed));
	SetInputAction(KEY_S, MoveDown, INPUT_DOWN, "Down", ArgsToCallArgs(2, camera, &camera_speed));

	msdfgl_font_t font = msdfgl_load_font(state->msdfgl_context, "resources/fonts/arial.ttf", 4.0, 2.0, state->atlas); /* range, scale, atlas (NULL creates a new one) */
	/* Loads characters 0-128 onto the textures. This is where all the GPU cycles went. */
	// msdfgl_generate_ascii(font);
	
	while (!state->quit)
	{
		EngineUpdate();
		UpdateCamera();

		glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawGradientV((Vector4){51, 76.5f, 153, 255}, (Vector4){178.5f, 229.5f, 255, 255}, state->camera.position.y / 100);
		DrawSDFTextWorld("DEUTSCHLANDtesting", font, 4, 0, 64.f, (Vector4){255, 255, 255, 255});
    DrawRect((Rectangle){0, 0, 2, 1}, (Vector4){125, 125, 125, 255});
    DrawRect((Rectangle){-3, 0, 2, 1}, (Vector4){255, 75, 75, 255});
    DrawRect((Rectangle){3, 0, 2, 1}, (Vector4){75, 75, 75, 255});
    DrawTexRect(rec, tex2.ID, 0.f);
    Vector3 text_size = MeasureWorldTextText(&text, 0);
    DrawRect((Rectangle){text.x + text_size.x / 2, text.y + text_size.y / 2 + text_size.z, text_size.x, text_size.y}, (Vector4){25, 25, 25, 50});
    DrawRect((Rectangle){state->mouse_world.x + text_size.x / 2, state->mouse_world.y + text_size.y / 2 + text_size.z, text_size.x, text_size.y}, (Vector4){25, 25, 25, 50});
    DrawUIRect((Rectangle){25 + text_size.x / 2, 125.f + text_size.y / 2 - text_size.z, text_size.x, text_size.y}, (Vector4){125, 125, 125, 50}, 0.f);
    DrawLine2DWorld((Vector2){0, 0}, state->mouse_world, (Vector4){255, 0, 0, 255});
    DrawCube((Vector3){1, 1.5, 0.5f}, (Vector3){1, 1, 1}, (Vector3){20, 0, 20}, tex1);
    DrawCube((Vector3){5, 2, 0.5f}, (Vector3){5, 1, 1}, (Vector3){40, 40.f, 20.f}, tex1);
    DrawUIRect((Rectangle){0, 0, 250*sinf(state->time), 250*sinf(state->time)}, (Vector4){255, 255, 255, 255}, state->time*200);
		DrawSDFText("based ingreedy -)v(-", font, 0, 0, 64.f, (Vector4){114, 24, 204, 255});
		EnginePresent();
	}
	EngineQuit();
	return 0;
}
