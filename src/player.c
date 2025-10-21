#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/player.h"
#include "../include/tilemap.h"
#include "../include/world.h"
#include "../include/config.h"

void init_player(player_t* player) {
    // visual
    player->state = IDLE;
    player->anim_speed = 0.008f;
    player->flipped = false;
    player->bb_size = 16;
    player->sprite_size = (Vector2) { 8, 8 }; // Bug, collision only works with some values
    player->sprite_offset = (Vector2) { 0, -5 }; // Bug, collision only works with some values

    player->texture = LoadTexture("assets/sprites/player.png");

    if (!IsTextureValid(player->texture)) {  printf("WARINING: Player texture loading failed"); return; }

    // physics
    player->position.x = 0.0f;
    player->position.y = 0.0f;

    player->velocity.x = 0.0f;
    player->velocity.y = 0.0f;
    
    player->acceleration = 1000.0f;
    player->friction = 0.001f;

    player->speed_cap = 100.0f;
}


Rectangle get_player_sprite(player_t* player, unsigned long total_ms) {
    int frame = (int)(total_ms*player->anim_speed) % 4;
    int size = player->bb_size; 
    
    int rect_width = size * (player->flipped ? -1 : 1);
    Rectangle rect = {
        frame*size,
        (int)player->state * size,
        rect_width, 
        size,
    };
    return rect;
}

void render_player(player_t* player, unsigned long total_ms) {
    Rectangle dest_rect = {
        player->position.x + player->sprite_offset.x,
        player->position.y + player->sprite_offset.y,
        player->bb_size,
        player->bb_size,
    };

    DrawTexturePro(player->texture, get_player_sprite(player, total_ms), dest_rect, (Vector2) { 0.0, 0.0 }, 0, WHITE);
}

void player_update_velocity_by_input(player_t* player) {
    float frameTime = GetFrameTime();

    Vector2 inputs = { 0.0f, 0.0f };
    if      (IsKeyDown(KEY_A)) { inputs.x = -1; }
    else if (IsKeyDown(KEY_D)) { inputs.x =  1; }

    if      (IsKeyDown(KEY_W)) { inputs.y = -1; }
    else if (IsKeyDown(KEY_S)) { inputs.y =  1; }


    // player->velocity = player->velocity + (inputs * player->acceleration * deltaTime);
    float acc_x_dt = player->acceleration * frameTime;
    Vector2 inp_x_ad = Vector2Scale(inputs, acc_x_dt);

    player->velocity = Vector2Add(player->velocity, inp_x_ad);

    // clamp velocity
    if (Vector2Length(player->velocity) < 0.01) { player->velocity = Vector2Zero(); return; }
    player->velocity.x = player->velocity.x > player->speed_cap ? player->speed_cap : player->velocity.x;
    player->velocity.x = player->velocity.x < -player->speed_cap ? -player->speed_cap : player->velocity.x;

    player->velocity.y = player->velocity.y > player->speed_cap ? player->speed_cap : player->velocity.y;
    player->velocity.y = player->velocity.y < -player->speed_cap ? -player->speed_cap : player->velocity.y;
}

Rectangle** get_col_tiles_around_player(player_t* player, tilemap_t* col_map) {
    const int ts = 16; // tile size
    // tilemap at position (0,0)
    
    Rectangle** close_walls = malloc(sizeof(Rectangle*) * 3); 
    if (!close_walls) { printf("[!] malloc failed"); exit(1); }
    for (int i = 0; i < 3; i++) {
        close_walls[i] = calloc((size_t)3, sizeof(Rectangle)); 
        if (!close_walls[i]) { printf("[!] calloc failed"); exit(1); }
    }

    Vector2 offsets[3][3] = {
        {(Vector2){-1.0f, -1.0f}, (Vector2){0.0f, -1.0f}, (Vector2){1.0f, -1.0f}},
        {(Vector2){-1.0f, 0.0f}, (Vector2){0.0f, 0.0f}, (Vector2){1.0f, 0.0f}},
        {(Vector2){-1.0f, 1.0f}, (Vector2){0.0f, 1.0f}, (Vector2){1.0f, 1.0f}},
    };

    for (int x = 0; x<3; x++) {
        for (int y = 0; y<3; y++) {
            // get player center
            int pmx = player->position.x + 8;
            int pmy = player->position.y + 8;
            
            // calculate near tiles
            int tile_x = pmx/(int)ts + offsets[y][x].x;
            int tile_y = pmy/(int)ts + offsets[y][x].y;
            
            // get tile id
            int collider_type = get_tile_at(col_map, tile_x, tile_y);

            if (collider_type == -1) {
                continue; // No collision (air)
            }
            
            // get subrect by tile id 
            Rectangle sub_rect = get_collision_sub_rect(collider_type, ts);

            // move subrect to tile position
            sub_rect.x += tile_x * ts;
            sub_rect.y += tile_y * ts;
            
            // save subrect to return array
            close_walls[y][x] = sub_rect; 
        }
    }

    return close_walls;
}

void player_move_by_velocity(player_t* player) {
    player->position = Vector2Add(player->position, Vector2Scale(player->velocity, GetFrameTime()));
}

int player_move_and_collide(player_t* player, world_t* world) {

    Rectangle** rects = get_col_tiles_around_player(player, world->col_map);

    int col = 0; // 0 = no col
                 // 1 = horizontal 
                 // 2 = vertical 
                 // 3 = both 

    int ts = 16; // tile size
    Vector2 offset = Vector2Scale(player->velocity, GetFrameTime());
    
    Vector2 p_ws = {
        // player sprite whitespace
        0.5f*(player->bb_size - player->sprite_size.x),
        0.5f*(player->bb_size - player->sprite_size.y),
    };

    // HORIZONTAL COMPONENT ----------
    player->position.x  += offset.x;

    // check collision
    float xpos = player->position.x;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (CheckCollisionRecs(rects[y][x], get_player_rect(player))) {
                // printf("Horizontal Collision \n");
                col += 1;
                // collision
                if (offset.x > 0) {
                    // collision on the right -> snap to left edge
                    player->velocity.x = 0;
                    player->position.x = rects[y][x].x - player->bb_size + p_ws.x;
                } 
                else if (offset.x < 0) {
                    // collision on the left -> snap to right edge
                    player->velocity.x = 0;
                    player->position.x = rects[y][x].x + rects[y][x].width - p_ws.x ;
                }
            }
        }
    }

    // VERTICAL COMPONENT --------
    player->position.y  += offset.y;

    // check collision
    float ypos = player->position.y;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (CheckCollisionRecs(rects[y][x], get_player_rect(player))) {
                // printf("Vertical Collision \n");
                col += 2;
                // collision
                if (offset.y > 0) {
                    // collision on the bottom -> snap to top edge
                    player->velocity.y = 0;
                    player->position.y = rects[y][x].y - player->bb_size + p_ws.y;
                } else if (offset.y <0) {
                    // collision on the top -> snap to bottom edge
                    player->velocity.y = 0;
                    player->position.y = rects[y][x].y + rects[y][x].height - p_ws.y;
                }
            }
        }
    }


    // Finish up
    free(rects);
    return col;
}

void player_apply_friction(player_t* player) {
    float frameTime = GetFrameTime();
    player->velocity = Vector2Scale(player->velocity, pow(player->friction, frameTime));
    // player->velocity = Vector2Scale(player->velocity, player->friction);
}

void player_face_moving_dir(player_t* player) {
    if (!player->velocity.x) { return; } 
    player->flipped = player->velocity.x < 0;
    return;
}

void player_update_state(player_t* player) {
    if (Vector2Length(player->velocity) > 10.0) {
        player->state = RUNNING;
    }
    else {
        player->state = IDLE;
    }
}

Rectangle get_player_rect(player_t* player) {
    int v_offset = 0.5f*(player->bb_size - player->sprite_size.y);
    int h_offset = 0.5f*(player->bb_size - player->sprite_size.x);

    return (Rectangle) {
        .x = player->position.x + v_offset,
        .y = player->position.y + h_offset,
        .width = player->sprite_size.x,
        .height = player->sprite_size.y,
    };
}

void update_player(player_t* player, world_t* world) {

    player_update_velocity_by_input(player);
    if (get_config_ptr()->player_collision) {
        player_move_and_collide(player, world);
    } else {
        player_move_by_velocity(player);
    }
    player_face_moving_dir(player);
    player_update_state(player);
    player_apply_friction(player);
}
