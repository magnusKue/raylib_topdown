#include <raylib.h>
#include <raymath.h>

#include "../include/player.h"
#include "../include/camera.h"

void init_camera(camera_t* camera, float zoom) {
    update_offset(camera);

    camera->cam.target = Vector2Zero();
    camera->cam.rotation = 0.0;
    camera->cam.zoom = zoom;
}

void update_camera(camera_t* camera, player_t* player) {
    target_player(camera, player);
    update_offset(camera);
}

void update_offset(camera_t* camera) {
    Vector2 offset = {
        (GetScreenWidth() / 2.0f),
        (GetScreenHeight() / 2.0f),
    };
    camera->cam.offset = offset;
}

void target_player(camera_t* camera, player_t* player) {
    int player_size = 16;
    Vector2 target = Vector2AddValue(player->position, 0.5*player_size);
    camera->cam.target = target;
}
