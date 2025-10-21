#include <raylib.h>
#include <stdlib.h>

#include "../include/tilemap.h"
#include "../include/world.h"
#include "../include/config.h"
#include "../include/player.h"

void init_world(world_t* world, char* collision_map) {
   world->col_map = load_tilemap(collision_map);
}


