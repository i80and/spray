#ifndef __SPRAY_SCENE_H__
#define __SPRAY_SCENE_H__

typedef struct _SPScene SPScene;

SPScene_new(void);
SPScene_attach(SPScene* self, SPEntity* entity);
SPScene_remove(SPScene* self, SPEntity* entity);
SPScene_find_children(SPScene* self, SPPoint center, double width, double height);
SPScene_draw(SPScene* self, SPStage* stage);
SPScene_free(SPScene* self);

#endif
