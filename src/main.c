#include <raylib.h>
#include <stdio.h>

#include "../include/window.h"
#include "../include/player.h"
#include "../include/camera.h"
#include "../include/tilemap.h"
#include "../include/world.h"

int game();
int test_tilemap();

int main() {
    game();
    // test_tilemap();
}

int test_tilemap() {
    InitWindow(640, 400, "My Game");
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

    // test tilemap
    tilemap_t* tmap = load_tilemap("assets/map01.txt");
}

int game() {
    unsigned long time_us = 0; // time in µs
    
    // create_tile

    player_t player;
    camera_t camera;
    world_t world;

    // Initialize components
    init_window("Game", /*RESOLUTION:*/1920/2, 1200/2, /*FPS:*/600);
    init_player(&player);
    init_camera(&camera, /*ZOOM*/ 3.0);
    init_world(&world, "assets/levels/maps/test_collision.csv");

    
    tileset_t* tileset_sunnyside = load_tileset(16, "assets/tilesheets/sunnyside.png");
    tileset_t* tileset_collision = load_tileset(16, "assets/tilesheets/collision_tilesheet.png");

    tilemap_t* tilemap_objects   = load_tilemap("assets/levels/maps/test_object.csv");
    tilemap_t* tilemap_ground    = load_tilemap("assets/levels/maps/test_ground.csv");

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
