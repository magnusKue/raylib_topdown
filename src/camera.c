#include <raylib.h>
#include <raymath.h>

#include "../include/player.h"
#include "../include/camera.h"

void init_camera(camera_t* camera, float zoom) {
    Vector2 offset = {
        (GetScreenWidth() * 0.3 / 2.0f) - 8,
        (GetScreenHeight() * 0.3 / 2.0f) - 8,
    };
    camera->cam.offset = offset;
    camera->cam.target = Vector2Zero();
    camera->cam.rotation = 0.0;
    camera->cam.zoom = zoom;
}

void update_camera(camera_t* camera, player_t* player) {
    camera->cam.target = player->position;
}
