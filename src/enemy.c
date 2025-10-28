# include <assert.h>
#include <stdio.h>

# include "../include/enemy.h"
# include "../include/player.h"
# include "../include/world.h"
# include "../include/config.h"

# include "../include/zombie_enemy.h"

enemy_t create_enemy(enemy_type_t type) {
   enemy_t new_enemy;

   switch (type) {
      case ZOMBIE:
         init_zombie_enemy(&new_enemy);
         break;
   }

   return new_enemy;

}

void update_enemy(enemy_t* enemy, player_t* player, world_t* world) {
    assert(enemy->set_vel_func != NULL);
    assert(enemy->move_func != NULL);
    assert(enemy->update_anim_func != NULL);

    enemy->set_vel_func(world, enemy, player);
    enemy->move_func(enemy, player, world);
    enemy->update_anim_func(enemy, player);

    entity_face_moving_dir(&(enemy->entity));
}

void update_enemies(enemy_t* enemies, int enemy_count, player_t* player, world_t* world) {
    for (int i = 0; i < enemy_count; i++) {
        update_enemy(&enemies[i], player, world);
    }
}

void render_enemy(enemy_t* enemy, world_t* world) {
    if (get_config_ptr()->render_paths && enemy->path.nodes) {
        DrawLineStrip(enemy->path.nodes, enemy->path.len, WHITE);   
        for (int i = 0; i < enemy->path.len; i++) {
            DrawCircleV(enemy->path.nodes[i], 1.0f, WHITE);
        }
    }

    render_entity(&enemy->entity, world);
}


void render_enemies(enemy_t* enemies, int enemy_count, world_t* world) {
    for (int i = 0; i < enemy_count; i++) {
        render_enemy(&enemies[i], world);
    }
}
