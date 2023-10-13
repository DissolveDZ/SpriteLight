void GBufferSetup(unsigned int *g_buffer, unsigned int *g_position, unsigned int *g_normal, unsigned int *g_albedo, unsigned int *depth, int screen_width, int screen_height)
{
    glGenFramebuffers(1, g_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, *g_buffer);
    glGenTextures(1, g_position);
    glBindTexture(GL_TEXTURE_2D, *g_position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *g_position, 0);
    // normal color buffer
    glGenTextures(1, g_normal);
    glBindTexture(GL_TEXTURE_2D, *g_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, *g_normal, 0);
    // color + specular color buffer
    glGenTextures(1, g_albedo);
    glBindTexture(GL_TEXTURE_2D, *g_albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, *g_albedo, 0);

    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);
    // create and attach depth buffer (renderbuffer)
    glGenRenderbuffers(1, depth);
    glBindRenderbuffer(GL_RENDERBUFFER, *depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screen_width, screen_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *depth);
    // check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("Framebuffer not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void PostProcessBuffer(unsigned int *post_process_fbo, unsigned int *post_process_color, unsigned int *depth, int screen_width, int screen_height)
{
    // use Create Framebuffer later
    glCreateFramebuffers(1, post_process_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, *post_process_fbo);
    glCreateTextures(GL_TEXTURE_2D, 1, post_process_color);
    glBindTexture(GL_TEXTURE_2D, *post_process_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *post_process_color, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *depth);
}

void BatchSetup()
{
    state->renderer.max_quads = 1000;
    state->renderer.max_vertices = state->renderer.max_quads * 4;
    state->renderer.max_indices = state->renderer.max_quads * 6;
    state->renderer.max_textures = 32;
    // glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &state->renderer.max_textures);

    glUseProgram(basic_shader.ID);
    int textures[32];
    for (int i = 0; i < 32; i++)
        textures[i] = i;

    glUniform1iv(glGetUniformLocation(basic_shader.ID, "textures"), 32, textures);
    glUseProgram(0);

    glCreateVertexArrays(1, &state->renderer.vao);
    glBindVertexArray(state->renderer.vao);

    glCreateBuffers(1, &state->renderer.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, state->renderer.vbo);
    glBufferData(GL_ARRAY_BUFFER, state->renderer.max_vertices * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, tex_coords));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, tex_id));
    glEnableVertexAttribArray(2);

    state->renderer.textures = malloc(state->renderer.max_textures * sizeof(u32));
    state->renderer.buffer_object = malloc(state->renderer.max_vertices * sizeof(Vertex));
    state->renderer.buffer_object_ptr = NULL;
    state->renderer.indices = malloc(state->renderer.max_indices * sizeof(u32));

    u32 offset = 0;
    for (size_t i = 0; i < state->renderer.max_indices; i += 6)
    {
        state->renderer.indices[i + 0] = 0 + offset;
        state->renderer.indices[i + 1] = 1 + offset;
        state->renderer.indices[i + 2] = 2 + offset;

        state->renderer.indices[i + 3] = 2 + offset;
        state->renderer.indices[i + 4] = 3 + offset;
        state->renderer.indices[i + 5] = 0 + offset;

        offset += 4;
    }

    glCreateBuffers(1, &state->renderer.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state->renderer.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(state->renderer.indices) * state->renderer.max_indices, state->renderer.indices, GL_STATIC_DRAW);

    glCreateTextures(GL_TEXTURE_2D, 1, &state->renderer.white);
    glBindTexture(GL_TEXTURE_2D, state->renderer.white);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    u32 color = 0xffffffff;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

    memset(state->renderer.textures, 0, state->renderer.max_textures * sizeof(u32));
    state->renderer.textures[0] = state->renderer.white;
}

void BeginBatch()
{
    state->renderer.buffer_object_ptr = state->renderer.buffer_object;
}

void EndBatch()
{
    GLsizeiptr size = (u8 *)state->renderer.buffer_object_ptr - (u8 *)state->renderer.buffer_object;
    glBindBuffer(GL_ARRAY_BUFFER, state->renderer.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, state->renderer.buffer_object);
}

void FlushBatch()
{
    UseShader(basic_shader);

    SetShaderMat4(basic_shader.ID, "projection", state->projection);
    SetShaderMat4(basic_shader.ID, "view", state->view);
    
    for (u32 i = 0; i < state->renderer.tex_index; i++)
        glBindTextureUnit(i, state->renderer.textures[i]);
    state->renderer.index_count = (state->renderer.vertex_count/4) * 6;

    glBindVertexArray(state->renderer.vao);
    glDrawElements(GL_TRIANGLES, state->renderer.index_count, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
        
    state->renderer.vertex_count = 0;
    state->renderer.tex_index = 1;
}