# pragma once

#include <raylib.h>

#include "../include/world.h"

typedef enum State {
    // index == spritesheet row
    IDLE=0,
    RUNNING=1,
} state_t;

typedef struct Player {
    // visual
    state_t state;
    float anim_speed;
    Texture2D texture;
    bool flipped;
    int bb_size; // bounding box
    Vector2 sprite_size;
    Vector2 sprite_offset;

    // physics
    Vector2 position;
    Vector2 velocity;
    float acceleration;
    float friction;
    float speed_cap;
} player_t;

void init_player(player_t* player);
void update_player(player_t* player, world_t* world);
void render_player(player_t* player, unsigned long total_ms, world_t* world);

Rectangle get_player_rect(player_t* player);
Rectangle** get_tiles_around_player(player_t* player, tilemap_t* col_map);

