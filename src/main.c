#include <raylib.h>
#include <stdio.h>

#include "../include/window.h"
#include "../include/player.h"
#include "../include/camera.h"

int main() {
    unsigned long time = 0;

    window_t window;
    player_t player;
    camera_t camera;

    // Initialize components
    init_window(&window, "Game", 800, 600, 0.3);
    init_player(&player);
    init_camera(&camera);

    while (!WindowShouldClose()) {
        // UPDATE
        update_player(&player);
        update_camera(&camera, &player);
        
        // RENDER
        start_render(&window, &camera);
        
            render_player(&player, time);

        render_to_window(&window, &camera);

        time += GetFrameTime() * 1000;
    }

    // cleanup_window(&window);
    return 0;
}
