#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <lauxlib.h>
#include <GL/gl.h>
#include <png.h>

#include "texture.h"
#include "primitives.h"

typedef enum {
	spTextureFmt_RGB=0,
	spTextureFmt_RGBA
	// TODO: Support BGR and compression
} spTextureFmt;

struct _SPTexture {
	spTexID texid;
	
	int width;
	int height;
};

SPTexture* SPTexture_new(int width, int height, spTexID texid) {
	SPTexture* texture = malloc(sizeof(struct _SPTexture));
	texture->texid = texid;
	texture->width = width;
	texture->height = height;
	
	return texture;
}

// Upload a texture to the graphics card
static GLuint upload_texture(spTextureFmt fmt, int width, int height, const ubyte* data) {
	spTexID tex = spTexID_Bad;
	
	if(NULL == data) {
		// WTF happened here?
		return tex;
	}
	
	// Initialize our texture ID
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	{
		const GLint lod = 0;
		const GLint border = 0;
		GLint internalfmt = 0;
		switch(fmt) {
			case spTextureFmt_RGB:
				internalfmt = GL_RGB;
				break;
			case spTextureFmt_RGBA:
				internalfmt = GL_RGBA;
				break;
			default:
				assert(false);
		}

		// Upload the texture data		
		glTexImage2D(GL_TEXTURE_2D, lod, internalfmt, width, height,
		             border, internalfmt, GL_UNSIGNED_BYTE, data);		             
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	printf("%d\n", tex);
	return tex;
}

// Pull a texture off the graphics card
// Load a PNG file, and send it to upload_texture
SPTexture* SPTexture_frompng(const char* path) {
	SPTexture* texture = NULL;
	spTexID texid = spTexID_Bad;
	FILE* pngf = fopen(path, "rb");
	
	uint32_t width = 0;
	uint32_t height = 0;
	ubyte* pixmap = NULL;
	png_byte** row_heads = NULL;
	const size_t SIG_LENGTH = 8;
	
	png_structp png_state = NULL;
	png_infop png_info = NULL;

	if(NULL == pngf) {
		return texture;
	}
	
	// Ensure that the given path really is a PNG file
	{
		ubyte signature[8];
		size_t bytes_read = fread(signature, sizeof(byte), SIG_LENGTH, pngf);
		if(bytes_read < 8) {
			fclose(pngf);
			printf("Error\n");
			return texture;
		}
		
		if(png_sig_cmp(signature, 0, SIG_LENGTH)) {
			fclose(pngf);
			return texture;
		}
	}
	
	// Read the PNG file
	png_state = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_info = png_create_info_struct(png_state);
	
	png_init_io(png_state, pngf);
	png_set_sig_bytes(png_state, SIG_LENGTH);
	
	if(setjmp(png_jmpbuf(png_state))) {	
		// Something went wrong while reading the PNG data
		png_destroy_read_struct(&png_state, &png_info, NULL);

		if(pixmap) {
			free(pixmap);
		}
		if(row_heads) {
			free(row_heads);
		}
		fclose(pngf);
		return spTexID_Bad;
	}
	
	// Prep work for reading the PNG 
	{
		png_read_info(png_state, png_info);
		width = png_get_image_width(png_state, png_info);
		height = png_get_image_height(png_state, png_info);
		
		// We use signed integers to correspond with glTexImage2D, but PNG uses uint32_t
		if(width >= INT_MAX || height >= INT_MAX) {
			return spTexID_Bad;
		}
		
		png_byte bit_depth = png_get_bit_depth(png_state, png_info);
		png_byte color_type = png_get_color_type(png_state, png_info);
		png_size_t stride = 0;

		// Ensure RGBA color
		if(color_type == PNG_COLOR_TYPE_PALETTE) {
			png_set_palette_to_rgb(png_state);
		}
		if(color_type == PNG_COLOR_TYPE_RGB) {
			png_set_add_alpha(png_state, 0xFF, PNG_FILLER_AFTER);
		}

		// Ensure exactly 8 bits of color
		if(bit_depth > 8) {
			png_set_strip_16(png_state);
		}
		else if(bit_depth < 8) {
			png_set_packing(png_state);
		}
		
		png_read_update_info(png_state, png_info);

		// Create blank, safe-ish row pointers that point inside a dense pixmap
		stride = png_get_rowbytes(png_state, png_info);
		pixmap = malloc(stride * height);
		row_heads = calloc(sizeof(byte*), height);
		for(uint32_t row=0; row<height; row++) {
			// I've read that OpenGL expects rows in the opposite order, but
			// this works fine
			row_heads[row] = pixmap + row*stride;
//			row_heads[row] = &(pixmap[(height-row-1)*stride]);
		}
		
		// Read in the PNG data
		png_read_image(png_state, row_heads);
		texid = upload_texture(spTextureFmt_RGBA, (GLsizei)width, (GLsizei)height, pixmap);
		
		// Goodness gracious, Spaceman!  What a mess to clean!
		png_destroy_read_struct(&png_state, &png_info, NULL);
		free(row_heads);
		free(pixmap);
	}
	
	fclose(pngf);

	texture = SPTexture_new(width, height, texid);

	return texture;
}

int SPTexture_get_width(SPTexture* self) {
	assert(self != NULL);

	return self->width;
}

int SPTexture_get_height(SPTexture* self) {
	assert(self != NULL);

	return self->height;
}

void SPTexture_free(SPTexture* self) {
	assert(self != NULL);

	glDeleteTextures(1, &self->texid);
	free(self);
}
