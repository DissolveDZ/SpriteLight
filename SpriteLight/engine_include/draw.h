// draws a quad with no matrices which will cover the whole screen
void DrawQuad();
// draws a rectangle in world-space with a given color
void DrawRect(Rectangle rec, Vector4 color);
// draws a screen-space rectangle, size in pixels
void DrawUIRect(Rectangle rec, Vector4 color);
// draws a textured rectangle
void DrawTexRect(Rectangle rec, u32 tex_id, float rotation);
// draws a textured rectangle with a tint
void DrawTexRectTint(Rectangle rec, u32 tex_id, float rotation, Vector4 tint);
// draws text in world-space
void DrawWorldText(char *text, Font *font, float x, float y, float scale, Vector4 color);
// draws text using the text struct
void DrawWorldTextText(Text text, Font *font);
// draws a Text using signed-distance-fields and other tricks to look really crisp
void DrawSDFText(const char *text, msdfgl_font_t font, float x, float y, float scale, Vector4 color);
// draws a Text using signed-distance-fields and other tricks to look really crisp
void DrawSDFTextWorld(const char *text, msdfgl_font_t font, float x, float y, float scale, Vector4 color);
// draws a basic screen-space text
void DrawText(char *text, Font *font, float x, float y, float scale, Vector4 color);
// draws a basic screen-space text using the text struct
// might make it return an array which can be used to offset individual characters
void DrawTextText(Text text, Font *font);
// draw only part of text, best used for animated text
void DrawSubText(char *text, Font *font, int count, float x, float y, float scale, Vector4 color);
// draws only part of text, best used for animated text using the text struct
void DrawSubTextText(Text *text, Font *font, int count);
// draws a textured cube in world-space
void DrawCube(Vector3 position, Vector3 scale, Vector3 rotation, Texture texture);
// draws a gradient which covers the whole screen
void DrawGradientV(Vector4 start, Vector4 end, float offset);
// draws a line
void DrawLine2DWorld(Vector2 start, Vector2 end, Vector4 color);
