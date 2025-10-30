#pragma once

# include "../include/entity.h"
# include "../include/player.h"
# include "../include/world.h"

typedef enum EnemyType {
    ZOMBIE=1,
} enemy_type_t;

typedef struct Enemy enemy_t;
typedef struct Enemy {
    entity_t entity;
    path_t path;

    int vision_radius;
    void* data;

    void (*execute_state)(world_t*, enemy_t*, player_t*);
    void (*update_state)(world_t*, enemy_t*, player_t*);

} enemy_t;


enemy_t create_enemy(enemy_type_t type);
void update_enemy(enemy_t* enemy, player_t* player, world_t* world);
void update_enemies(enemy_t* enemies, int enemy_count, player_t* player, world_t* world);

void render_enemy(enemy_t* enemy, world_t* world);
void render_enemies(enemy_t* enemies, int enemy_count, world_t* world);
