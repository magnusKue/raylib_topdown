#include <raylib.h>
#include <stdio.h>

#include "../include/window.h"
#include "../include/player.h"
#include "../include/camera.h"
#include "../include/tilemap.h"
#include "../include/world.h"

int start_game() {
    unsigned long time_us = 0; // time in µs
    
    // create_tile

    player_t player;
    camera_t camera;
    world_t world;

    // Initialize components
    init_window("Game", /*RESOLUTION:*/1920/2, 1200/2, /*FPS:*/600);
    init_player(&player);
    init_camera(&camera, /*ZOOM*/ 3.0);
    init_world(&world, "assets/levels/test_collision.csv");
    printf("[!] Initialized successfully!\n\n");
    
    tileset_t* tileset_sunnyside = load_tileset(16, "assets/tilesheets/sunnyside.png");
    tileset_t* tileset_collision = load_tileset(16, "assets/tilesheets/collision_tilesheet.png");
    printf("[!] Loaded tilesets successfully!\n\n");

    tilemap_t* tilemap_objects   = load_tilemap("assets/levels/test_object.csv");
    tilemap_t* tilemap_ground    = load_tilemap("assets/levels/test_ground.csv");
    printf("\n[!] Loaded tilemaps successfully!\n\n");

    while (!WindowShouldClose()) {
        // UPDATE
        update_player(&player, &world);
        update_camera(&camera, &player);

        // RENDER
        start_render(&camera);
            render_tilemap(tilemap_ground, tileset_sunnyside);
            render_tilemap(tilemap_objects, tileset_sunnyside);

            render_player(&player, (int)time_us/1000);

        render_to_window(&camera);

        time_us += GetFrameTime() * 1000000;
    }

    // cleanup_window(&window);
    return 0;
}
