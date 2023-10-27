#ifndef SALAD_H
#define SALAD_H

void InitHashTable(size_t initial_capacity);
void FreeResources();
Resource *LoadResource(const char* file_name);
Resource *GetResource(int index);
void RemoveResource(int index);
Texture LoadTexture(const char *texture_name);
Shader Load_Shader(const char *vertex_name, const char *fragment_name);
unsigned int Hash(const char *str);
void HandleError(const char *message, ...);

#endif /* SALAD_H */