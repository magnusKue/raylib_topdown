#include <raylib.h>
#include <stdio.h>

#include "../include/window.h"
#include "../include/player.h"
#include "../include/camera.h"
#include "../include/tilemap.h"

int main() {
    unsigned long time_us = 0; // time in µs
    
    // create_tile

    window_t window;
    player_t player;
    camera_t camera;

    // Initialize components
    init_window(&window, "Game", 800*2, 600*2, 0.3);
    init_player(&player);
    init_camera(&camera);

    while (!WindowShouldClose()) {
        // UPDATE
        update_player(&player);
        update_camera(&camera, &player);
        
        // RENDER
        start_render(&window, &camera);
        
            render_player(&player, (int)time_us/1000);

        render_to_window(&window, &camera);

        time_us += GetFrameTime() * 1000000;
    }

    // cleanup_window(&window);
    return 0;
}
