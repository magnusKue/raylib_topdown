# pragma once

#include <raylib.h>

#include "../include/world.h"
#include "../include/entity.h"

typedef enum State {
    // index == spritesheet row
    IDLE=0,
    RUNNING=1,
} state_t;

typedef struct Player {
    // visual
    float anim_speed;

    entity_t entity;
} player_t;

void init_player(player_t* player);
void update_player(player_t* player, world_t* world);
void render_player(player_t* player, world_t* world);
