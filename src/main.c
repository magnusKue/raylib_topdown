#include <raylib.h>
#include <stdio.h>

#include "../include/window.h"
#include "../include/player.h"
#include "../include/camera.h"
#include "../include/tilemap.h"

int game();
int test_tilemap();

int main() {
    game();
    // test_tilemap();
}

int test_tilemap() {
    InitWindow(800, 600, "My Game");
    printf("\n--- GAME RUNNING ---\n");

    // test tile 
    tile_t* tile = create_tile(16, 32, 64);
    printf("test tile: Pos(%f, %f)\n", tile->rect.x, tile->rect.y);

    // test tileset
    tileset_t* ts = load_tileset(16, "assets/test_tileset.png");
    
    for (int x = 0; x < ts->num_tiles; x++) {
        printf("Tile at pos (%.1f|%.1f) with size (%.1f)\n", 
            ts->tiles[x]->rect.x,
            ts->tiles[x]->rect.y,
            ts->tiles[x]->rect.width,
            ts->tiles[x]->rect.height
        );
    }
}

int game() {
    unsigned long time_us = 0; // time in µs
    
    // create_tile

    window_t window;
    player_t player;
    camera_t camera;

    // Initialize components
    init_window(&window, "Game", /*RESOLUTION:*/800, 600, /*SCALER:*/0.3, /*FPS:*/600);
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
