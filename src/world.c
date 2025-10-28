#include <raylib.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/tilemap.h"
#include "../include/world.h"
#include "../include/config.h"
#include "../include/enemy.h"

#define ENEMY_CAP 1024

void init_world(world_t* world, char* collision_map) {
   world->col_map = load_tilemap(collision_map);
   world->afield = (afield_t)generate_astar_field(
         world->col_map->rows, 
         world->col_map->columns, 
         world->col_map->tmap
   );
   world->enemies = malloc(sizeof(enemy_t) * ENEMY_CAP);
   assert(world->enemies);
   world->enemy_counter = 0;
}

void push_enemy_to_world(world_t* world, enemy_t enemy) {
   if (world->enemy_counter >= ENEMY_CAP) {
      printf("[!!] MAXIUM ENEMY CAP OVERSTEPPED\n");
      return;
   }

   world->enemies[world->enemy_counter++] = enemy;
}

void spawn_enemy(enemy_type_t type, world_t* world, Vector2 position) {
   enemy_t enemy = create_enemy(type);
   enemy.entity.position = position;
   push_enemy_to_world(world, enemy);
}


Vector2 world_to_tilemap_coord(Vector2 world_pos) {
   Vector2 tile_coord;
   tile_coord.x = (int)(floor(world_pos.x / TILE_SIZE));
   tile_coord.y = (int)(floor(world_pos.y / TILE_SIZE));
   return tile_coord;
}

Vector2 tilemap_to_world_coord(Vector2 tile_pos) {
   // scales to top left corner
   Vector2 world_coord;
   world_coord = Vector2Scale(tile_pos, TILE_SIZE); 
   return world_coord;
}

path_t world_get_path(world_t* world, Vector2 start, Vector2 target, int max_searches) {
   // reset all field values for a clean new search
   reset_field(&world->afield, world->col_map->tmap);
   
   // if target is in a collision tile try neighboring cells
   Vector2 goal = target;
   if (world->col_map->tmap[(int)target.y][(int)target.x] != -1) {
      Vector2 offsets[8] = {
         // axis aligned checks first
         (Vector2){ 0.0f,  1.0f},
         (Vector2){ 1.0f,  0.0f},
         (Vector2){-1.0f,  0.0f},
         (Vector2){ 0.0f, -1.0f},

         // diagonals next
         (Vector2){-1.0f, -1.0f},
         (Vector2){-1.0f,  1.0f},
         (Vector2){ 1.0f, -1.0f},
         (Vector2){ 1.0f,  1.0f},
      };

      bool better_pos_found = false;
      for (int i = 0; i < 8; i++) {
         Vector2 test_pos = (Vector2) {
            .x = target.x + offsets[i].x,
            .y = target.y + offsets[i].y,
         };

         // bounds check
         if (test_pos.x < 0 || test_pos.y < 0 || 
               test_pos.x >= world->col_map->columns || 
               test_pos.y >= world->col_map->rows
         ) {
            // out of bounds
            continue;
         }
         // printf("checking out %d %d\n", (int)test_pos.x, (int)test_pos.y);
         if (world->col_map->tmap[(int)test_pos.y][(int)test_pos.x] == -1) {
            // valid goal found
            goal = (Vector2) { (int)test_pos.x, (int)test_pos.y };
            // printf("goal: %d %d\n\n", (int)goal.x, (int)goal.y);

            better_pos_found = true;
            break;
         }     
      }

      if (!better_pos_found) {
         return (path_t) {
            .len = -1,
            .nodes = NULL,
            .target_pos = (Vector2) { -1, -1 },
            .index = -1,
            .finished = true,
            .current_dir = (Vector2) { -1, -1 },
            .current_speed = -1,
         }; 
      }
   }

   int path_len = 0;
   Vector2* path = get_a_star_path(&world->afield, start, goal, max_searches, &path_len);
   
   // scale path from tile to world positions
   for (int i = 0; i < path_len; i++) {
      path[i] = tilemap_to_world_coord(path[i]);
      path[i].x += TILE_SIZE * 0.5;
      path[i].y += TILE_SIZE * 0.5;
   }

   return (path_t) {
      .len = path_len,
      .nodes = path,
      .target_pos = goal,
      .index = 1, // skip start node, we should already be there
      .finished = false,
      .current_dir = (Vector2) { 0.0f, 0.0f },
      .current_speed = 0,
   };
}

