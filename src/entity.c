#include <stdlib.h>
#include <assert.h>
#include "entity.h"
#include "texture.h"

struct _SPEntity {
	SPTexture* texture;
	SPPoint loc;
};

SPEntity* SPEntity_new(SPPoint pos) {
	SPEntity* entity = calloc(1, sizeof(struct _SPEntity));
	
	entity->loc = pos;
	
	return entity;
}

void SPEntity_set_position(SPEntity* self, SPPoint pos) {
	assert(self != NULL);
	
	self->loc = pos;
}

SPPoint SPEntity_get_position(SPEntity* self) {
	assert(self != NULL);

	return self->loc;
}

void SPEntity_set_texture(SPEntity* self, SPTexture* tex) {
	assert(self != NULL);

	self->texture = tex;
}

void SPEntity_free(SPEntity* self) {
	assert(self != NULL);
	
	free(self);
}
