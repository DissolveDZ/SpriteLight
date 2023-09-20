void LightingPass()
{
}

void DrawQuad()
{
    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void DrawRect(Rectangle rec, Vector4 color)
{
    // Skip matrix when not moving
    UseShader(basic_shader);

    SetShaderMat4(basic_shader.ID, "projection", state->projection);
    SetShaderMat4(basic_shader.ID, "view", state->view);
    SetShaderBool(basic_shader.ID, "use_color", true);
    SetShaderBool(basic_shader.ID, "use_normals", false);
    SetShaderVec4(basic_shader.ID, "color", (vec4){color.r / 255, color.g / 255, color.b / 255, color.a / 255});
    SetShaderVec3(basic_shader.ID, "view_pos", (vec3){state->camera.position.x, state->camera.position.y, state->camera.position.z});
    SetShaderFloat(basic_shader.ID, "exposure", 1);
    SetShaderBool(basic_shader.ID, "hdr", true);

    glm_mat4_identity(state->model);
    glm_translate(state->model, (vec3){rec.x, rec.y, 0.0f});
    glm_scale(state->model, (vec3){rec.width, rec.height, 1});
    SetShaderMat4(basic_shader.ID, "model", state->model);

    glBindVertexArray(plane_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void DrawTexRect(Rectangle rec)
{
    // Skip matrix when not moving
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rec.texture.ID);

    UseShader(basic_shader);
    SetShaderInt(basic_shader.ID, "tex", 0);
    SetShaderMat4(basic_shader.ID, "projection", state->projection);
    SetShaderMat4(basic_shader.ID, "view", state->view);
    SetShaderBool(basic_shader.ID, "use_color", false);
    SetShaderBool(basic_shader.ID, "use_normals", false);
    SetShaderVec3(basic_shader.ID, "view_pos", (vec3){state->camera.position.x, state->camera.position.y, state->camera.position.z});
    SetShaderFloat(basic_shader.ID, "exposure", 1);
    SetShaderBool(basic_shader.ID, "hdr", true);

    glm_mat4_identity(state->model);
    glm_translate(state->model, (vec3){rec.x, rec.y, 0.0f});
    glm_scale(state->model, (vec3){rec.width, rec.height, 1});
    SetShaderMat4(basic_shader.ID, "model", state->model);

    glBindVertexArray(plane_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void DrawUIRect(Rectangle rec, Vector4 color)
{
    glm_mat4_identity(state->model);
    glm_translate(state->model, (vec3){rec.x, rec.y, 0.f});
    glm_translate(state->model, (vec3){0.5f * rec.width, 0.5f * rec.height, 0.f});
    glm_rotate(state->model, glm_rad(0), (vec3){0.0f, 0.0f, 1.f});
    glm_translate(state->model, (vec3){-0.5f * rec.width, -0.5f * rec.height, 0.f});
    glm_scale(state->model, (vec3){rec.width/2, rec.height/2, 0.5f});
    UseShader(basic_screen_space_shader);
    SetShaderMat4(basic_screen_space_shader.ID, "model", state->model);
    SetShaderMat4(basic_screen_space_shader.ID, "projection", state->ortho_projection);
    SetShaderBool(basic_screen_space_shader.ID, "use_color", true);
    SetShaderVec4(basic_screen_space_shader.ID, "color", (vec4){color.r / 255, color.g / 255, color.b / 255, color.a / 255});

    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
    glBindVertexArray(text_vao);
    for (char i = 0; i != strlen(text.text); i++)
    {
        TextCharacter ch;
        if (font == NULL)
            ch = default_chars[text.text[i]];
        else
            ch = font->loaded_chars[text.text[i]];
        float xpos = text.x + ch.bearing[0] * text.scale;
        float ypos = text.y - (ch.size[1] - ch.bearing[1]) * text.scale;
        float w = ch.size[0] * text.scale;
        float h = ch.size[1] * text.scale;

        float vertices[4][5] = {
            {xpos, ypos + h, 0.f, 0.0f, 0.0f},
            {xpos, ypos, 0.f, 0.0f, 1.0f},
            {xpos + w, ypos + h, 0.f, 1.0f, 0.0f},
            {xpos + w, ypos, 0.f, 1.0f, 1.0f}};

        glBindTexture(GL_TEXTURE_2D, ch.texture_id);

        glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        text.x += (ch.advance >> 6) * text.scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
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
        glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        text.x += (ch.advance >> 6) * text.scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}