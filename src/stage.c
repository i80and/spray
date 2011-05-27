#include <assert.h>
#include <GL/gl.h>
#include <SDL.h>
#include "stage.h"

void SPPoint_set(SPPoint* self, double x, double y) {
	assert(self != NULL);

	self->x = x;
	self->y = y;
}

void SPPoint_move(SPPoint* self, double x, double y) {
	assert(self != NULL);

	self->x += x;
	self->y += y;
}

SPPoint SPPoint_add(SPPoint* p1, SPPoint* p2) {
	SPPoint result;
	SPPoint_set(&result, (p1->x+p2->x), (p1->y+p2->y));
	return result;
}

SPPoint SPPoint_subtract(SPPoint* p1, SPPoint* p2) {
	assert(p1 != NULL && p2 != NULL);

	SPPoint result;
	SPPoint_set(&result, (p1->x-p2->x), (p1->y-p2->y));
	return result;
}

void SPPoint_copy(SPPoint* src, SPPoint* dest) {
	assert(src != NULL && dest != NULL);

	dest->x = src->x;
	dest->y = src->y;
}

void SPColor_set(SPColor* self, byte red, byte green, byte blue, byte alpha) {
	assert(self != NULL);

	self->red = red;
	self->green = green;
	self->blue = blue;
	self->alpha = alpha;	
}

void SPColor_copy(SPColor* src, SPColor* dest) {
	assert(src != NULL && dest != NULL);

	dest->red = src->red;
	dest->green = src->green;
	dest->blue = src->blue;
	dest->alpha = src->alpha;
}

static inline void _SPColor_activate(SPColor* color) {
	assert(color != NULL);
	
	glColor4f(color->red, color->green, color->blue, color->alpha);
}

struct _SPStage {
	SDL_Window* win;
	SDL_GLContext gl;
	SPPoint camera_center;
};

SPStage* SPStage_new(const char* title, int width, int height) {
	SPStage* self = malloc(sizeof(struct _SPStage));
	self->win = SDL_CreateWindow(title,
	                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                            width, height,
	                            SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
	self->gl = SDL_GL_CreateContext(self->win);
	SDL_GL_SetSwapInterval(1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width, width, -height, height, 0.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	
	// Set the camera to the default location
	SPPoint_set(&self->camera_center, 0, 0);
	
	// VSync
	SDL_GL_SetSwapInterval(1);

	// Enable textures and alpha blending
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor4f(1.0, 1.0, 1.0, 1.0);

	return self;
}

void SPStage_center(SPStage* self, SPPoint* center) {
	assert(self != NULL);

	SPPoint_copy(center, &self->camera_center);
}

void SPStage_draw_line(SPStage* self, SPPoint* start, SPPoint* end, SPColor* color, float width, int samples) {
	assert(self != NULL);
	assert(start != NULL);
	assert(end != NULL);
	assert(color != NULL);

	SPColor cur_color;
	
	// Disable texturing for this line
	glBindTexture(GL_TEXTURE_2D, 0);

	// Translate our points into a local coordinate system around the camera.
	SPPoint localstart = SPPoint_subtract(start, &self->camera_center);
	SPPoint localend = SPPoint_subtract(end, &self->camera_center);
	
	// Anti-aliasing is currently crude: increasingly wide and transparent lines are drawn over
	// each other.  Note that this can be trivially optimized to death down the road.
	SPColor_copy(color, &cur_color);
	for(int i = 0; i < samples; i++) {
		glLineWidth(width);
		_SPColor_activate(&cur_color);

		glBegin(GL_LINES);
			glVertex2d(localstart.x, localstart.y);
			glVertex2d(localend.x, localend.y);
		glEnd();
		
		cur_color.alpha /= 2;
		width += 1;
	}
}

void SPStage_draw_tex(SPStage* self, SPTexture* tex, SPColor* mix, SPPoint* pos) {
/*	assert(self != NULL);
	assert(tex != NULL);
	assert(mix != NULL);
	assert(pos != NULL);

	_SPColor_activate(mix);
	
	// Translate the point into a local coordinate system around the camera.
	SPPoint localpos = SPPoint_subtract(pos, &self->camera_center);
	
	glBindTexture(GL_TEXTURE_2D, tex);
	
	glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i(width, 0);
		glVertex2i(width, height);
		glVertex2i(0, height);
	glEnd();*/
}

void SPStage_finalize(SPStage* self) {
	assert(self != NULL);
	
	// Finish rendering and swap the buffers
	glFlush();
	SDL_GL_SwapWindow(self->win);

	// Clean up whatever is currently in the buffer
	glClear(GL_COLOR_BUFFER_BIT);
}

void SPStage_free(SPStage* self) {
	assert(self != NULL);

	SDL_GL_DeleteContext(self->gl);
	SDL_DestroyWindow(self->win);
	free(self);
}
