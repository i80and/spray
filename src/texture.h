// Represent an OpenGL texture handle in Lua, opening from image files as necessary

#ifndef __SPRAY_TEXTURE_H__
#define __SPRAY_TEXTURE_H__

typedef struct _SPTexture SPTexture;

typedef unsigned int spTexID;
#define spTexID_Bad 0

SPTexture* SPTexture_new(int width, int height, spTexID texid);
SPTexture* SPTexture_frompng(const char* path);

int SPTexture_get_width(SPTexture* self);
int SPTexture_get_height(SPTexture* self);

void SPTexture_free(SPTexture* self);

#endif
