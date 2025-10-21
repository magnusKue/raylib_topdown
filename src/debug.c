#include <raylib.h>
#include <stdlib.h>

# include "../include/debug.h"
# include "../include/config.h"

void  render_colliders(player_t* player, world_t* world) {
   if (get_config_ptr()->render_colliders) {
      // PLAYER
      DrawRectangleLinesEx(get_player_rect(player), 2.0f, WHITE);
      // TILEMAP
      Rectangle** rects = get_col_tiles_around_player(player, world->col_map);
      render_collision_tiles(rects);
      free(rects);
   }
}
