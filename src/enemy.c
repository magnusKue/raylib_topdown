# include <assert.h>

# include "../include/enemy.h"
# include "../include/player.h"
# include "../include/world.h"

void update_enemy(enemy_t* enemy, player_t* player, world_t* world) {
    assert(enemy->set_vel_func != NULL);
    assert(enemy->move_func != NULL);

    enemy->set_vel_func(enemy, player);
    enemy->move_func(&(enemy->entity), world);
}

