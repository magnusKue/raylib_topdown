# pragma once

#include "../include/enemy.h"
#include "../include/player.h"
#include "../include/world.h"

void target_player(world_t* world, enemy_t* enemy, player_t* player);
void init_zombie_enemy(enemy_t* enemy);
void update_animation_path(enemy_t* enemy);
void update_animation_velocity(enemy_t* enemy);
