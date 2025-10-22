# pragma once

#include <raylib.h>
#include "../include/entity.h"

typedef struct Camera {
    Camera2D cam;
} camera_t;


void init_camera(camera_t* camera, float zoom);
void update_camera(camera_t* camera, entity_t* entity);
void update_offset(camera_t* camera);
void target_entity(camera_t* camera, entity_t* entity);
