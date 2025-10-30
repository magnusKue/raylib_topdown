# pragma once

#include "../include/enemy.h"
#include "../include/player.h"
#include "../include/world.h"

void pathfind_player(world_t* world, enemy_t* enemy, player_t* player);

void init_zombie_enemy(enemy_t* enemy);

void zombie_execute_state(world_t* world, enemy_t* enemy, player_t* player);
void zombie_update_state(world_t* world, enemy_t* enemy, player_t* player);
