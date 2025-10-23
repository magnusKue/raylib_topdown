#include "../include/test_enemy.h"
#include "../include/enemy.h"
#include "../include/entity.h"
#include "../include/player.h"

# include <raymath.h>

typedef enum Test_Enemy_States {
    TEST_ENEMY_IDLE = 0,
} test_enemy_states;


void init_test_enemy(enemy_t* enemy) {
    enemy->set_vel_func = &target_player;
    enemy->move_func = &entity_move_and_collide;
}

void target_player(enemy_t* enemy, player_t* player) {
    enemy->entity.velocity = Vector2Scale(Vector2Normalize(Vector2Subtract(player->entity.position, enemy->entity.position)), 25.0f);
}
