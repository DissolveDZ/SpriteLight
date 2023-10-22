void LightingPass()
{
}

void DrawQuad()
{
	// glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

Vertex *CreateQuad(Vertex *target, float x, float y, float width, float height, float rotation, float tex)
{
	float half_width = width * 0.5f;
	float half_height = height * 0.5f;

	mat4 transform = GLM_MAT4_IDENTITY_INIT;
	glm_translate(transform, (vec3){x, y, 0.0f});
	if (rotation != 0.f)
		glm_rotate(transform, glm_rad(rotation), (vec3){0.0f, 0.0f, 1.0f});
	glm_scale(transform, (vec3){width, height, 1.0f});

	vec3s vertices[4] = {
		{-half_width, -half_height, 0.0f},
		{half_width, -half_height, 0.0f},
		{half_width, half_height, 0.0f},
		{-half_width, half_height, 0.0f}};

	for (int i = 0; i < 4; i++)
	{
		vec3 temp = {vertices[i].x, vertices[i].y, vertices[i].z};
		glm_mat4_mulv3(transform, temp, 1, target->position.raw);
		float tex_x = (i == 0 || i == 3) ? 0.0f : 1.0f;
		float tex_y = (i == 0 || i == 1) ? 0.0f : 1.0f;

		target->tex_coords = (Vector2){tex_x, tex_y};
		target->tex_id = tex;
		target++;
	}
	return target;
}

// called by other function using the shader, drawing with a custom shader would be it's own function
void DrawRect(Rectangle rec, Texture tex, float rotation, unsigned int shader)
{
	if (state->renderer.current_shader != shader)
	{
		if (state->renderer.current_shader != -1)
		{
			EndFlushBatch(&state->renderer.batches[state->renderer.current_batch]);
			BeginBatch();
		}
		state->renderer.current_shader = shader;
	}

	// Check if there is a batch with the same shader.
	Batch *batch = NULL;
	for (int i = 0; i < state->renderer.batch_count; i++)
	{
		if (state->renderer.batches[i].shader.ID == shader)
		{
			batch = &state->renderer.batches[i];
			state->renderer.current_batch = i;
			break;
		}
	}

	if (batch == NULL)
	{
		state->renderer.batch_count++;
		if (state->renderer.batch_count >= state->renderer.max_batches - 1)
		{
			state->renderer.max_batches *= 1.5f;
			state->renderer.batches = realloc(state->renderer.batches, state->renderer.max_batches * sizeof(Batch));
		}

		batch = &state->renderer.batches[state->renderer.batch_count - 1];
		batch->shader.ID = shader;
		state->renderer.current_batch = state->renderer.batch_count - 1;
	}

	// Check if the batch is full, and if so, flush it.
	if (batch->vertex_count > state->renderer.max_vertices || state->renderer.tex_index > 31)
	{
		EndFlushBatch(&state->renderer.batches[state->renderer.current_batch]);
		BeginBatch();
	}

	// Handle texture indexing.
	float texture_index = 0.0f;
	for (u32 i = 1; i < state->renderer.tex_index; i++)
	{
		if (state->renderer.textures[i] == tex.ID)
		{
			texture_index = (float)i;
			break;
		}
	}

	if (texture_index == 0.0f)
	{
		texture_index = (float)state->renderer.tex_index;
		state->renderer.textures[state->renderer.tex_index] = tex.ID;
		state->renderer.tex_index++;
	}

	// Create and add the quad to the batch.
	batch->buffer_object_ptr = CreateQuad(batch->buffer_object_ptr, rec.x, rec.y, rec.width, rec.height, rotation, texture_index);
	batch->vertex_count += 4;
}

void DrawTexRectTint(Rectangle rec, Vector4 tint)
{
	// Skip matrix when not moving
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rec.texture.ID);

	UseShader(basic_shader);
	SetShaderInt(basic_shader.ID, "tex", 0);
	SetShaderMat4(basic_shader.ID, "projection", state->projection);
	SetShaderMat4(basic_shader.ID, "view", state->view);
	SetShaderVec4(basic_shader.ID, "tint", (vec4){tint.r / 255, tint.g / 255, tint.b / 255, tint.a / 255});
	SetShaderBool(basic_shader.ID, "use_color", false);
	SetShaderBool(basic_shader.ID, "use_normals", false);
	SetShaderVec3(basic_shader.ID, "view_pos", (vec3){state->camera.position.x, state->camera.position.y, state->camera.position.z});
	SetShaderFloat(basic_shader.ID, "exposure", 1);
	SetShaderBool(basic_shader.ID, "hdr", true);

	glm_mat4_identity(state->model);
	glm_translate(state->model, (vec3){rec.x, rec.y, 0.0f});
	glm_scale(state->model, (vec3){rec.width, rec.height, 1});
	SetShaderMat4(basic_shader.ID, "model", state->model);

	// glBindVertexArray(plane_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void DrawTexRect(Rectangle rec)
{
	DrawTexRectTint(rec, (Vector4){255, 255, 255, 255});
}

void DrawUIRect(Rectangle rec, Vector4 color)
{
	UseShader(basic_screen_space_shader);
	glm_mat4_identity(state->model);
	glm_translate(state->model, (vec3){rec.x, rec.y, 0.f});
	glm_translate(state->model, (vec3){0.5f * rec.width, 0.5f * rec.height, 0.f});
	glm_rotate(state->model, glm_rad(0), (vec3){0.0f, 0.0f, 1.f});
	glm_translate(state->model, (vec3){-0.5f * rec.width, -0.5f * rec.height, 0.f});
	glm_scale(state->model, (vec3){rec.width / 2, rec.height / 2, 0.5f});
	SetShaderMat4(basic_screen_space_shader.ID, "model", state->model);
	SetShaderMat4(basic_screen_space_shader.ID, "projection", state->ortho_projection);
	SetShaderBool(basic_screen_space_shader.ID, "use_color", true);
	SetShaderVec4(basic_screen_space_shader.ID, "color", (vec4){color.r / 255, color.g / 255, color.b / 255, color.a / 255});
	SetShaderVec4(basic_screen_space_shader.ID, "tint", (vec4){1, 1, 1, 1});

	// glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void DrawUITexRect(Rectangle rec)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rec.texture.ID);
	UseShader(basic_screen_space_shader);
	SetShaderInt(basic_screen_space_shader.ID, "tex", 0);
	glm_mat4_identity(state->model);
	glm_translate(state->model, (vec3){rec.x, rec.y, 0.f});
	glm_translate(state->model, (vec3){0.5f * rec.width, 0.5f * rec.height, 0.f});
	glm_rotate(state->model, glm_rad(0), (vec3){0.0f, 0.0f, 1.f});
	glm_translate(state->model, (vec3){-0.5f * rec.width, -0.5f * rec.height, 0.f});
	glm_scale(state->model, (vec3){rec.width / 2, rec.height / 2, 0.5f});
	SetShaderMat4(basic_screen_space_shader.ID, "model", state->model);
	SetShaderMat4(basic_screen_space_shader.ID, "projection", state->ortho_projection);
	SetShaderBool(basic_screen_space_shader.ID, "use_color", false);

	// glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void DrawLine2DWorld(Vector2 start, Vector2 end, Vector4 color)
{
	UseShader(basic_shader);
	SetShaderMat4(basic_shader.ID, "projection", state->projection);
	SetShaderMat4(basic_shader.ID, "view", state->view);
	SetShaderBool(basic_shader.ID, "use_color", true);
	SetShaderVec4(basic_shader.ID, "color", (vec4){color.r / 255, color.g / 255, color.b / 255, color.a / 255});
	SetShaderMat4(basic_shader.ID, "model", (mat4)GLM_MAT4_IDENTITY_INIT);
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
void DrawTextText(Text text, Font *font)
{
	UseShader(text_shader);
	SetShaderInt(text_shader.ID, "tex", 0);
	SetShaderMat4(text_shader.ID, "projection", state->ortho_projection);
	SetShaderVec4(text_shader.ID, "text_color", (vec4){text.color.r / 255, text.color.g / 255, text.color.b / 255, text.color.a / 255});
	glActiveTexture(GL_TEXTURE0);
	// glBindVertexArray(text_vao);
	// glBindBuffer(GL_ARRAY_BUFFER, text_vbo);

	Vector2 text_offset = (Vector2){text.x, text.y};
	size_t text_length = strlen(text.text);

	float highest_in_row = 0.f;
	for (size_t i = 0; i < text_length; i++)
	{
		TextCharacter ch;
		if (font == NULL)
			ch = default_chars[text.text[i]];
		else
			ch = font->loaded_chars[text.text[i]];

		float w = ch.size[0] * text.scale;
		float h = ch.size[1] * text.scale;

		if (h > highest_in_row)
			highest_in_row = h;

		if (text.text[i] == '\n')
		{
			text_offset.x = text.x;
			text_offset.y += highest_in_row;
			// currently doesn't account for the fact that \n shouldn't be added
			// but it creates nice spacing so i'll leave it in
			continue;
		}

		float xpos = text_offset.x + ch.bearing[0] * text.scale;
		float ypos = -(text_offset.y + (ch.size[1] - ch.bearing[1]) * text.scale);

		float vertices[4][5] = {
			{-xpos, ypos + h, 0.f, 0.0f, 0.0f},
			{-xpos, ypos, 0.f, 0.0f, 1.0f},
			{-(xpos + w), ypos + h, 0.f, 1.0f, 0.0f},
			{-(xpos + w), ypos, 0.f, 1.0f, 1.0f}};

		glBindTexture(GL_TEXTURE_2D, ch.texture_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		text_offset.x += (ch.advance >> 6) * text.scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DrawCube(Vector3 position, Vector3 scale, Vector3 rotation, Texture texture)
{
	glEnable(GL_DEPTH_TEST);
	UseShader(basic_shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	SetShaderMat4(basic_shader.ID, "projection", state->projection);
	SetShaderMat4(basic_shader.ID, "view", state->view);
	SetShaderBool(basic_shader.ID, "use_color", false);
	SetShaderBool(basic_shader.ID, "use_normals", true);
	SetShaderVec3(basic_shader.ID, "view_pos", (vec3){state->camera.position.x, state->camera.position.y, state->camera.position.z});
	SetShaderFloat(basic_shader.ID, "exposure", 1);
	SetShaderBool(basic_shader.ID, "hdr", true);

	glm_mat4_identity(state->model);
	mat4 temp;
	glm_mat4_identity(temp);
	versor quater;
	glm_quat_identity(quater);
	mat4 idendidy;
	glm_mat4_identity(idendidy);
	mat4 transformx;
	glm_mat4_identity(transformx);
	glm_rotate(transformx, glm_rad(rotation.x), (vec3){1.f, 0.f, 0.f});
	mat4 transformy;
	glm_mat4_identity(transformy);
	glm_rotate(transformy, glm_rad(rotation.y), (vec3){0.f, 1.f, 0.f});
	mat4 transformz;
	glm_mat4_identity(transformz);
	glm_rotate(transformz, glm_rad(rotation.z), (vec3){0.f, 0.f, 1.f});
	mat4 rotationmat;
	glm_mat4_identity(rotationmat);
	glm_mul(transformy, transformx, temp);
	glm_mul(temp, transformz, rotationmat);

	glm_translate(state->model, position.raw);
	glm_mat4_mul(state->model, rotationmat, state->model);
	glm_scale(state->model, scale.raw);
	// glm_rotate(model, glm_rad((float)SDL_GetTicks64() / 50), rotation);
	SetShaderMat4(basic_shader.ID, "model", state->model);
	// glBindVertexArray(cube_vao);
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

	// glBindVertexArray(quad_vao);
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
	// glBindVertexArray(text_vao);
	text.scale *= 0.001f;
	// commented for text-centering, might use later for UI objects like buttons etc.
	// when changing text length the text would go up and down which is usually not wanted since text should be able to change dynamically best suited for static/centered text
	/*
	float offset_y = 0;
	for (char i = 0; i != strlen(text.text); i++)
	{
		TextCharacter ch;
		if (font == NULL)
			ch = default_chars[text.text[i]];
		else
			ch = font->loaded_chars[text.text[i]];
		float ypos  = (ch.size[1] - ch.bearing[1]) * text.scale;
		if (!offset_y)
			offset_y = ypos;
		if (ypos < offset_y)
			offset_y = ypos;
	}
	text.y += offset_y;
	*/
	for (char i = 0; i != strlen(text.text); i++)
	{
		TextCharacter ch;
		if (font == NULL)
			ch = default_chars[text.text[i]];
		else
			ch = font->loaded_chars[text.text[i]];
		float w = ch.size[0] * text.scale;
		float h = ch.size[1] * text.scale;
		if (i == 0)
			text.x -= ch.bearing[0] * text.scale;
		float xpos = text.x + ch.bearing[0] * text.scale;
		float ypos = text.y - (ch.size[1] - ch.bearing[1]) * text.scale;
		float vertices[4][5] = {
			{xpos, ypos + h, 0.f, 0.0f, 0.0f},
			{xpos, ypos, 0.f, 0.0f, 1.0f},
			{xpos + w, ypos + h, 0.f, 1.0f, 0.0f},
			{xpos + w, ypos, 0.f, 1.0f, 1.0f}};
		glBindTexture(GL_TEXTURE_2D, ch.texture_id);
		// glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		text.x += (ch.advance >> 6) * text.scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}