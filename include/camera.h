#pragma once

#include <raylib.h>
#include "../include/player.h"

typedef struct Camera {
    Camera2D cam;
} camera_t;


void init_camera(camera_t* camera, float zoom);
void update_camera(camera_t* camera, player_t* player);
void update_offset(camera_t* camera);
void target_player(camera_t* camera, player_t* player);
