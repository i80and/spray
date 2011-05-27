#ifndef __SPRAY_STAGE_H__
#define __SPRAY_STAGE_H__

#include <stdint.h>
#include "primitives.h"
#include "texture.h"

typedef struct {
	double x;
	double y;
} SPPoint;
/*! Set a point's position absolutely. */
void SPPoint_set(SPPoint* self, double x, double y);
/*! Move a point relative to its current location. */
void SPPoint_move(SPPoint* self, double x, double y);
/*! Return the addition of p1 and p2. */
SPPoint SPPoint_add(SPPoint* p1, SPPoint* p2);
/*! Return the subtraction of p1 and p2. */
SPPoint SPPoint_subtract(SPPoint* p1, SPPoint* p2);
/*! Copy src into dest. */
void SPPoint_copy(SPPoint* src, SPPoint* dest);

typedef struct {
	float red;
	float green;
	float blue;
	float alpha;
} SPColor;
/*! Set a color. */
void SPColor_set(SPColor* self, byte red, byte green, byte blue, byte alpha);
void SPColor_copy(SPColor* src, SPColor* dest);

typedef struct _SPStage SPStage;
SPStage* SPStage_new(const char* title, int width, int height);
/*! Center this stage's camera on the given point. */
void SPStage_center(SPStage* self, SPPoint* center);
/*! Draw a smoothed line. */
void SPStage_draw_line(SPStage* self, SPPoint* start, SPPoint* end, SPColor* color, float width, int samples);
/*! Draw the given texture with dimensions at x,y */
void SPStage_draw_tex(SPStage* self, SPTexture* tex, SPColor* mix, SPPoint* pos);
void SPStage_finalize(SPStage* self);
void SPStage_free(SPStage* self);

#endif
