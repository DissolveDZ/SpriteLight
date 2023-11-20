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
	//Texture tex1 = LoadTexture("cube.png");
	//Texture tex2 = LoadTexture("vedl.png");

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

	msdfgl_font_t font = msdfgl_load_font(state->msdfgl_context, "resources/fonts/Antonio-Bold.ttf", 4.0, 2.0, state->atlas); /* range, scale, atlas (NULL creates a new one) */
	/* Loads characters 0-128 onto the textures. This is where all the GPU cycles went. */
	// msdfgl_generate_ascii(font);
	
	while (!state->quit)
	{
		EngineUpdate();
		UpdateCamera();

		glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DrawLine2DWorld((Vector2){0, 0}, state->mouse_world, (Vector4){255, 0, 0, 255});
		DrawSDFTextWorld("DEUTSCHLANDtesting", font, 0, 0, 64.f, (Vector4){255, 255, 255, 255});
		DrawSDFText("based ingreedy -)v(-", font, 0, 0, 64.f, (Vector4){114, 24, 204, 255});
		EnginePresent();
	}
	EngineQuit();
	return 0;
}
