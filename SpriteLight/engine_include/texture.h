#pragma once

typedef struct Texture
{
    unsigned int ID;
    char *path;
    char *type;
    int width;
    int height;
} Texture;

Texture LoadTexture(const char *path);
SDL_Surface *LoadSDLImage(char *path);