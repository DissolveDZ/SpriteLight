#include "engine.h"

void DrawQuad()
{
	glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void DrawRect(Rectangle rec, Vector4 color)
{
	// Skip matrix when not moving
	UseShader(general_shader);

	SetShaderMat4(general_shader.ID, "projection", state->projection);
	SetShaderMat4(general_shader.ID, "view", state->view);
	SetShaderBool(general_shader.ID, "use_color", true);
	SetShaderBool(general_shader.ID, "use_normals", false);
	SetShaderVec4(general_shader.ID, "color", (vec4){color.r / 255, color.g / 255, color.b / 255, color.a / 255});
	SetShaderVec3(general_shader.ID, "view_pos", (vec3){state->camera.position.x, state->camera.position.y, state->camera.position.z});
	SetShaderFloat(general_shader.ID, "exposure", 1);
	SetShaderBool(general_shader.ID, "hdr", true);

	glm_mat4_identity(state->model);
	glm_translate(state->model, (vec3){rec.x, rec.y, 0.0f});
	glm_scale(state->model, (vec3){rec.width, rec.height, 1});
	SetShaderMat4(general_shader.ID, "model", state->model);

	glBindVertexArray(plane_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void DrawTexRectTint(Rectangle rec, u32 tex_id, float rotation, Vector4 tint)
{
	// Skip matrix when not moving
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id);

	UseShader(general_shader);
	SetShaderInt(general_shader.ID, "tex", 0);
	SetShaderMat4(general_shader.ID, "projection", state->projection);
	SetShaderMat4(general_shader.ID, "view", state->view);
	SetShaderVec4(general_shader.ID, "tint", (vec4){tint.r / 255, tint.g / 255, tint.b / 255, tint.a / 255});
	SetShaderBool(general_shader.ID, "use_color", false);
	SetShaderBool(general_shader.ID, "use_normals", false);
	SetShaderVec3(general_shader.ID, "view_pos", (vec3){state->camera.position.x, state->camera.position.y, state->camera.position.z});

	glm_mat4_identity(state->model);
	glm_translate(state->model, (vec3){rec.x, rec.y, 0.0f});
	glm_rotate(state->model, glm_rad(rotation), (vec3){0.0f, 0.0f, 1.0f});
	glm_scale(state->model, (vec3){rec.width, rec.height, 1});
	SetShaderMat4(general_shader.ID, "model", state->model);

	glBindVertexArray(plane_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void DrawTexRect(Rectangle rec, u32 tex_id, float rotation)
{
	DrawTexRectTint(rec, tex_id, rotation, (Vector4){{255, 255, 255, 255}});
}

void DrawUIRect(Rectangle rec, Vector4 color)
{
	UseShader(ui_shader);
	glm_mat4_identity(state->model);
	glm_translate(state->model, (vec3){rec.x, rec.y, 0.f});
	glm_translate(state->model, (vec3){0.5f * rec.width, 0.5f * rec.height, 0.f});
	glm_rotate(state->model, glm_rad(0), (vec3){0.0f, 0.0f, 1.f});
	glm_translate(state->model, (vec3){-0.5f * rec.width, -0.5f * rec.height, 0.f});
	glm_scale(state->model, (vec3){rec.width / 2, rec.height / 2, 0.5f});
	SetShaderMat4(ui_shader.ID, "model", state->model);
	SetShaderMat4(ui_shader.ID, "projection", state->ortho_projection);
	SetShaderBool(ui_shader.ID, "use_color", true);
	SetShaderVec4(ui_shader.ID, "color", (vec4){color.r / 255, color.g / 255, color.b / 255, color.a / 255});
	SetShaderVec4(ui_shader.ID, "tint", (vec4){1, 1, 1, 1});

	glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void DrawUITexRect(Rectangle rec, u32 tex_id)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	UseShader(ui_shader);
	SetShaderInt(ui_shader.ID, "tex", 0);
	glm_mat4_identity(state->model);
	glm_translate(state->model, (vec3){rec.x, rec.y, 0.f});
	glm_translate(state->model, (vec3){0.5f * rec.width, 0.5f * rec.height, 0.f});
	glm_rotate(state->model, glm_rad(0), (vec3){0.0f, 0.0f, 1.f});
	glm_translate(state->model, (vec3){-0.5f * rec.width, -0.5f * rec.height, 0.f});
	glm_scale(state->model, (vec3){rec.width / 2, rec.height / 2, 0.5f});
	SetShaderMat4(ui_shader.ID, "model", state->model);
	SetShaderMat4(ui_shader.ID, "projection", state->ortho_projection);
	SetShaderBool(ui_shader.ID, "use_color", false);

	glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void DrawLine2DWorld(Vector2 start, Vector2 end, Vector4 color)
{
	UseShader(general_shader);
	SetShaderMat4(general_shader.ID, "projection", state->projection);
	SetShaderMat4(general_shader.ID, "view", state->view);
	SetShaderBool(general_shader.ID, "use_color", true);
	SetShaderVec4(general_shader.ID, "color", (vec4){color.r / 255, color.g / 255, color.b / 255, color.a / 255});
	SetShaderMat4(general_shader.ID, "model", (mat4)GLM_MAT4_IDENTITY_INIT);
	glBindVertexArray(line_vao);
	glBindBuffer(GL_ARRAY_BUFFER, line_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line_vertices), (float[]){start.x, start.y, 0, end.x, end.y, 0});
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glBindVertexArray(0);
}

void DrawText(char *text, Font *font, float x, float y, float scale, Vector4 color)
{
	DrawTextText((Text){text, x, y, scale, color}, font);
}

TextCharacter CalculateCharacterInfo(TextureAtlas atlas, char character)
{
	TextCharacter info;

	info.size.x = atlas.character_width;
	info.size.y = atlas.character_height;
	info.bearing.x = (character % atlas.num_columns) * atlas.character_width;
	info.bearing.y = (character / atlas.num_columns) * atlas.character_height;
	info.advance = atlas.character_width;

	return info;
}

void DrawTextText(Text text, Font *font)
{
	UseShader(text_shader);
	SetShaderInt(text_shader.ID, "tex", 0);
	SetShaderMat4(text_shader.ID, "projection", state->ortho_projection);
	SetShaderVec4(text_shader.ID, "text_color", (vec4){text.color.r / 255, text.color.g / 255, text.color.b / 255, text.color.a / 255});
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(text_vao);
	glBindBuffer(GL_ARRAY_BUFFER, text_vbo);

	Vector2 text_offset = (Vector2){{text.x, text.y}};
	size_t text_length = strlen(text.text);

	float highest_in_row = 0.f;
	for (size_t i = 0; i < text_length; i++)
	{
		TextCharacter ch;
		/*
		if (font == NULL)
			ch = default_chars[text.text[i]];
		else
			ch = font->loaded_chars[text.text[i]];
		*/
		ch = CalculateCharacterInfo(font->texture_atlas, text.text[i]);

		float w = 500; // ch.size.x * text.scale; // Update: Use ch.size.x instead of ch.size[0]
		float h = 500; // ch.size.y * text.scale; // Update: Use ch.size.y instead of ch.size[1]

		if (h > highest_in_row)
			highest_in_row = h;

		if (text.text[i] == '\n')
		{
			text_offset.x = text.x;
			text_offset.y += highest_in_row;
			continue;
		}

		float xpos = text_offset.x + ch.bearing.x * text.scale;					 // Update: Use ch.bearing.x instead of ch.bearing[0]
		float ypos = -(text_offset.y + (ch.size.y - ch.bearing.y) * text.scale); // Update: Use ch.size.y and ch.bearing.y

		float vertices[4][5] = {
			{-xpos, ypos + h, 0.f, 0.0f, 1.0f},
			{-xpos, ypos, 0.f, 0.0f, 0.0f},
			{-(xpos + w), ypos + h, 0.f, 1.0f, 1.0f},
			{-(xpos + w), ypos, 0.f, 1.0f, 0.0f}};

		glBindTexture(GL_TEXTURE_2D, font->texture_atlas.id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		text_offset.x += ch.advance * text.scale; // Update: Remove the bitshift (>> 6)
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DrawCube(Vector3 position, Vector3 scale, Vector3 rotation, Texture texture)
{
	glEnable(GL_DEPTH_TEST);
	UseShader(general_shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	SetShaderMat4(general_shader.ID, "projection", state->projection);
	SetShaderMat4(general_shader.ID, "view", state->view);
	SetShaderBool(general_shader.ID, "use_color", false);
	SetShaderBool(general_shader.ID, "use_normals", true);
	SetShaderVec3(general_shader.ID, "view_pos", (vec3){state->camera.position.x, state->camera.position.y, state->camera.position.z});
	SetShaderFloat(general_shader.ID, "exposure", 1);
	SetShaderBool(general_shader.ID, "hdr", true);
	SetShaderVec4(general_shader.ID, "tint", (vec4){1, 1, 1, 1});

	glm_mat4_identity(state->model);
	mat4 rotationmat = GLM_MAT4_IDENTITY_INIT;
  glm_rotate(rotationmat, glm_rad(rotation.x), (vec3){1.0f, 0.0f, 0.0f});
  glm_rotate(rotationmat, glm_rad(rotation.y), (vec3){0.0f, 1.0f, 0.0f});
  glm_rotate(rotationmat, glm_rad(rotation.z), (vec3){0.0f, 0.0f, 1.0f});

	glm_translate(state->model, position.raw);
	glm_mat4_mul(state->model, rotationmat, state->model);
	glm_scale(state->model, scale.raw);
	// glm_rotate(model, glm_rad((float)SDL_GetTicks() / 50), rotation);
	SetShaderMat4(general_shader.ID, "model", state->model);
	glBindVertexArray(cube_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 24);
	glBindVertexArray(0);
	glDisable(GL_DEPTH_TEST);
}

void DrawGradientV(Vector4 start, Vector4 end, float offset)
{
	UseShader(gradient_shader);
	SetShaderVec4(gradient_shader.ID, "start", (vec4){start.r / 255, start.g / 255, start.b / 255, start.a / 255});
	SetShaderVec4(gradient_shader.ID, "end", (vec4){end.r / 255, end.g / 255, end.b / 255, end.a / 255});
	SetShaderFloat(gradient_shader.ID, "offset", offset);

	glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void DrawWorldText(char *text, Font *font, float x, float y, float scale, Vector4 color)
{
	DrawWorldTextText((Text){text, x, y, scale, color}, font);
}

void DrawSubTextText(Text *text, Font *font, int count)
{
	DrawSubText(text->text, font, count, text->x, text->y, text->scale, text->color);
}

void DrawSubText(char *text, Font *font, int count, float x, float y, float scale, Vector4 color)
{
	if (!count || count > strlen(text))
		return;
	char *temp = malloc(count + 1);
	memcpy(temp, text, count);
	temp[count] = '\0';
	DrawText(temp, font, x, y, scale, color);
	free(temp);
}

void DrawWorldTextText(Text text, Font *font)
{
	UseShader(text_shader_world);
	SetShaderInt(text_shader_world.ID, "tex", 0);
	SetShaderMat4(text_shader_world.ID, "projection", state->projection);
	SetShaderMat4(text_shader_world.ID, "view", state->view);
	glm_mat4_identity(state->model);
	SetShaderMat4(text_shader_world.ID, "model", state->model);
	SetShaderVec4(text_shader_world.ID, "text_color", (vec4){text.color.r / 255, text.color.g / 255, text.color.b / 255, text.color.a / 255});
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(text_vao);
	text.scale *= 0.005f;

	for (int i = 0; i < strlen(text.text); i++)
	{
		Vector2 text_offset = (Vector2){{text.x, text.y}};
		
		TextCharacter cur_char = font->characters[(int)text.text[i]];
		
		float w = cur_char.size.x;
		float h = cur_char.size.y;
	
		float LetterX = cur_char.bearing.x; // Offset from the left edge of the character
		float LetterY = cur_char.bearing.y; // Offset from the top edge of the character

		float x0 = LetterX / font->texture_atlas.resolution;
		float y0 = (LetterY + font->texture_atlas.resolution) / font->texture_atlas.resolution;
		float y1 = (LetterY + font->texture_atlas.resolution-h) / font->texture_atlas.resolution;
		float x1 = (LetterX + w) / font->texture_atlas.resolution;

		w *= text.scale;
		h *= text.scale;

		// Define the vertices for the quad
		float vertices[4][5] = {
	  	{text_offset.x, text_offset.y + h, 0.f, x0, y0},
	   	{text_offset.x, text_offset.y, 0.f, x0, y1},
    	{text_offset.x + w, text_offset.y + h, 0.f, x1, y0},
  	  {text_offset.x + w, text_offset.y, 0.f, x1, y1}
		};

		glBindTexture(GL_TEXTURE_2D, font->texture_atlas.id); // Use the texture from the TextureAtlas
		glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		text.x += (cur_char.advance) * text.scale; // Remove the bitshift (>> 6)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
