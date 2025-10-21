# pragma once

#include "../include/tilemap.h"


typedef struct World {
    tilemap_t* col_map;
} world_t;

void init_world(world_t* world, char* collision_map);

