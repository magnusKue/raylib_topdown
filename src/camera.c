#include <raylib.h>
#include <raymath.h>

#include "../include/entity.h"
#include "../include/camera.h"

void init_camera(camera_t* camera, float zoom) {
    update_offset(camera);

    camera->cam.target = Vector2Zero();
    camera->cam.rotation = 0.0;
    camera->cam.zoom = zoom;
}

void update_camera(camera_t* camera, entity_t* entity) {
    target_entity(camera, entity);
    update_offset(camera);
}

void update_offset(camera_t* camera) {
    Vector2 offset = {
        (GetScreenWidth() / 2.0f),
        (GetScreenHeight() / 2.0f),
    };
    camera->cam.offset = offset;
}

void target_entity(camera_t* camera, entity_t* entity) {
    Vector2 target = Vector2AddValue(entity->position, 0.5*entity->bb_size);
    camera->cam.target = target;
}
