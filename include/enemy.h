#pragma once



# include "../include/entity.h"
# include "../include/player.h"
# include "../include/world.h"

typedef struct Enemy enemy_t;
typedef struct Enemy {
    entity_t entity;

    void (*set_vel_func)(enemy_t*, player_t*);
    int (*move_func)(entity_t*, world_t*);

} enemy_t;


void update_enemy(enemy_t* enemy, player_t* player, world_t* world);
