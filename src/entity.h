#ifndef __SPRAY_ENTITY_H__
#define __SPRAY_ENTITY_H__

#include <stdint.h>
#include "primitives.h"
#include "texture.h"
#include "stage.h"

#define SP_MAX_TEXTURES 8

typedef struct _SPEntity SPEntity;
/*! Create a new entity at the given location. */
SPEntity* SPEntity_new(SPPoint pos);
/*! Set self's absolute position in the world. */
void SPEntity_set_position(SPEntity* self, SPPoint pos);
/*! Get self's current absolute position. */
SPPoint SPEntity_get_position(SPEntity* self);
/*! Set self's texture. */
void SPEntity_set_texture(SPEntity* self, SPTexture* tex);
/*! Draw this entity to the given SPStage. */
void SPEntity_draw(SPEntity* self, SPStage* stage);
/*! Free any memory allocated by this entity. */
void SPEntity_free(SPEntity* self);

#endif
