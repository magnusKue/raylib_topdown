#include <raylib.h>
#include <stdlib.h>

# include "../include/debug.h"
# include "../include/config.h"
# include "../include/enemy.h"


void render_entity_collider(entity_t* entity, world_t* world) {
    if (get_config_ptr()->render_colliders) {
        float bottom = get_entity_bottom(entity);
        Vector2 center = get_entity_center(entity);
        DrawCircleV((Vector2) { center.x, bottom}, 1.0, BLACK);
    

       if (get_config_ptr()->render_colliders) {
          // PLAYER
          DrawRectangleLinesEx(get_entity_rect(entity), 1.0f, RED);
          // TILEMAP
          Rectangle** rects = get_col_tiles_around_entity(entity, world->col_map);
          render_collision_tiles(rects);
          free(rects);
       }
   }
}

void  render_colliders(player_t* player, world_t* world) {
    render_entity_collider(&player->entity, world);

    for (int i = 0; i < world->enemy_counter; i++) {
      render_entity_collider(&world->enemies[i].entity, world);
    }
}


void read_debug_input() {
        if (IsKeyPressed(KEY_TAB)) { get_config_ptr()->render_colliders ^= 1;  }
        if (IsKeyPressed(KEY_P)) { get_config_ptr()->render_paths ^= 1;  }

        if (IsKeyPressed(KEY_F)) { get_config_ptr()->player_collision ^= 1;  }

        if (IsKeyPressed(KEY_ONE)) { get_config_ptr()->render_ground ^= 1;  }
        if (IsKeyPressed(KEY_TWO)) { get_config_ptr()->render_objects ^= 1;  }
        if (IsKeyPressed(KEY_THREE)) { get_config_ptr()->render_tips ^= 1;  }
}
