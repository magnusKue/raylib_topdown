#include <raylib.h>

#include "../include/tilemap.h"
#include "../include/world.h"

void init_world(world_t* world, char* collision_map) {
   world->col_map = load_tilemap(collision_map);
}
