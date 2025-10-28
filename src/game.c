#include <raylib.h>
#include <stdio.h>

#include "../include/window.h"
#include "../include/player.h"
#include "../include/camera.h"
#include "../include/tilemap.h"
#include "../include/world.h"
#include "../include/renderer.h"
#include "../include/config.h"
#include "../include/debug.h"
#include "../include/enemy.h"
#include "../include/zombie_enemy.h"

int start_game() {
    player_t player;
    camera_t camera;
    world_t world;

    // Initialize components
    init_window("Game", /*RESOLUTION:*/1920/2, 1200/2, /*FPS:*/600);
    init_render_buffer();
    init_player(&player);
    init_camera(&camera, /*ZOOM*/ 5.0);
    init_world(&world, "assets/levels/test_collision.csv");
    printf("[!] Initialized successfully!\n\n");
    
    tileset_t* tileset_sunnyside = load_tileset(16, "assets/tilesheets/sunnyside.png");
    tileset_t* tileset_collision = load_tileset(16, "assets/tilesheets/collision_tilesheet.png");
    printf("[!] Loaded tilesets successfully!\n\n");

    tilemap_t* tilemap_tips   = load_tilemap("assets/levels/test_tips.csv");
    tilemap_t* tilemap_objects   = load_tilemap("assets/levels/test_objects.csv");
    tilemap_t* tilemap_ground    = load_tilemap("assets/levels/test_ground.csv");
    printf("\n[!] Loaded tilemaps successfully!\n\n");
    
    spawn_enemy(ZOMBIE, &world, Vector2Zero());

    while (!WindowShouldClose()) {
        // UPDATE
        update_player(&player, &world);
        update_camera(&camera, &(player.entity));
        update_enemies(world.enemies, world.enemy_counter, &player, &world);

        read_debug_input();

        // RENDER
        start_render(&camera);
            if (get_config_ptr()->render_ground) { render_tilemap(tilemap_ground, tileset_sunnyside); }
            if (get_config_ptr()->render_objects) { render_tilemap(tilemap_objects, tileset_sunnyside); }

            render_enemies(world.enemies, world.enemy_counter);
            render_player(&player);

            ysort_and_render_to_screen();

            if (get_config_ptr()->render_tips) { render_tilemap(tilemap_tips, tileset_sunnyside); }
            if (get_config_ptr()->render_colliders) { render_tilemap(world.col_map, tileset_collision); }

            render_colliders(&player, &world);
        
        render_to_window(&camera);

        // FINISH
        clear_render_buffer(); // turning this off looks really cool
    }

    return 0;
}
