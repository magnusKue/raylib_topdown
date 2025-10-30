# pragma once

#include "../include/tilemap.h"
#include "../include/astar.h"

typedef struct Enemy enemy_t;
typedef enum EnemyType enemy_type_t;

typedef struct World {
    tilemap_t* col_map;
    afield_t afield;

    enemy_t* enemies;
    int enemy_counter;
} world_t;

typedef struct Path {
    int len;
    Vector2* nodes;
    Vector2 target_pos;
    int index;

    bool finished;
    Vector2 current_dir;
    bool currently_moving;
} path_t;

void init_world(world_t* world, char* collision_map);

void spawn_enemy(enemy_type_t type, world_t* world, Vector2 position);

Vector2 world_to_tilemap_coord(Vector2 world_pos);
Vector2 tilemap_to_world_coord(Vector2 tile_pos);

path_t world_get_path(world_t* world, Vector2 start, Vector2 target, int max_searches);
