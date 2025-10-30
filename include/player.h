# pragma once

#include <raylib.h>
#include <sys/time.h>

#include "../include/world.h"
#include "../include/entity.h"
#include "../include/item.h"

typedef enum State {
    // index == spritesheet row
    IDLE=0,
    RUNNING=1,
} state_t;

typedef struct Player {
    item_t* current_item;

    // visual
    float anim_speed;
    timeval_t last_damage;
    Texture2D no_arms_tex;

    entity_t entity;
} player_t;

void hurt_player(player_t* player, int damage);
void equip_item(player_t* player, item_t* item);
void unequip_item(player_t* player);

void init_player(player_t* player);
void update_player(player_t* player, world_t* world);
void render_player(player_t* player, world_t* world);
