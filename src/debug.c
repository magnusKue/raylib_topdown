#include <raylib.h>
#include <stdlib.h>

# include "../include/debug.h"
# include "../include/config.h"

void  render_colliders(player_t* player, world_t* world) {
   if (get_config_ptr()->render_colliders) {
      // PLAYER
      Color col = get_config_ptr()->player_collision ? RED : BLUE;
      DrawRectangleLinesEx(get_entity_rect(&(player->entity)), 1.0f, col);
      // TILEMAP
      Rectangle** rects = get_col_tiles_around_entity(&(player->entity), world->col_map);
      render_collision_tiles(rects);
      free(rects);
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
