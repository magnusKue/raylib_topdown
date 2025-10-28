# pragma once

#include "../include/enemy.h"
#include "../include/player.h"
#include "../include/world.h"

void pathfind_player(world_t* world, enemy_t* enemy, player_t* player);
void init_zombie_enemy(enemy_t* enemy);
void update_animation_from_path(enemy_t* enemy, player_t* player);
void update_animation_from_velocity(enemy_t* enemy, player_t* player);
