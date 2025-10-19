#include <raylib.h>
#include <raymath.h>

#include "../include/player.h"
#include "../include/camera.h"

void init_camera(camera_t* camera) {
    Vector2 offset = {
        GetScreenHeight() / 2.0f,
        GetScreenWidth() / 2.0f,
    };
    camera->cam.offset = Vector2Zero();
    camera->cam.target = Vector2Zero();
    camera->cam.rotation = 0.0;
    camera->cam.zoom = 1.0;
}

void update_camera(camera_t* camera, player_t* player) {
    // camera->cam.target = player->position;
}
