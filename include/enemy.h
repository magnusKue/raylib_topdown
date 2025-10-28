#pragma once

# include "../include/entity.h"
# include "../include/player.h"
# include "../include/world.h"

typedef struct Enemy enemy_t;
typedef struct Enemy {
    entity_t entity;
    path_t path;

    int vision_radius;

    void (*set_vel_func)(world_t*, enemy_t*, player_t*);
    int (*move_func)(enemy_t*, player_t*, world_t*);
    void (*update_anim_func)(enemy_t*);

} enemy_t;


void update_enemy(enemy_t* enemy, player_t* player, world_t* world);
void render_enemy(enemy_t* enemy);
