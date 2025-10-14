#pragma once

#include <raylib.h>
#include "../include/player.h"

typedef struct Camera {
    Camera2D cam;
} camera_t;


void init_camera(camera_t* camera);
void update_camera(camera_t* camera, player_t* player);
