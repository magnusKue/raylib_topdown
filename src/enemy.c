# include <assert.h>
#include <stdio.h>

# include "../include/enemy.h"
# include "../include/player.h"
# include "../include/world.h"
# include "../include/config.h"

void update_enemy(enemy_t* enemy, player_t* player, world_t* world) {
    assert(enemy->set_vel_func != NULL);
    assert(enemy->move_func != NULL);
    assert(enemy->update_anim_func != NULL);

    enemy->set_vel_func(world, enemy, player);
    enemy->move_func(enemy, player, world);
    enemy->update_anim_func(enemy);

    entity_face_moving_dir(&(enemy->entity));
}

void render_enemy(enemy_t* enemy) {
    render_entity(&enemy->entity);

    if (get_config_ptr()->render_paths && enemy->path.nodes) {
        DrawLineStrip(enemy->path.nodes, enemy->path.len, WHITE);   
        DrawCircleV(tilemap_to_world_coord(enemy->path.target_pos), 2.0f, BLUE);
    }
}
