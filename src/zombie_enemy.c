#include "../include/zombie_enemy.h"
#include "../include/movement.h"
#include "../include/enemy.h"
#include "../include/entity.h"
#include "../include/player.h"
#include "../include/anim.h"

# include <raymath.h>
# include <assert.h>
# include <stdio.h>

#define MAX_TARGET_POS_CHANGE 16.0f /*In pixels, must be > TILE_SIZE !!!*/

typedef enum Zombie_Enemy_States {
    ZOMBIE_ENEMY_IDLE = 0,
    ZOMBIE_ENEMY_MOVE = 1,
    ZOMBIE_ENEMY_ATTACK = 2,
} zombie_enemy_states;


void init_zombie_enemy(enemy_t* enemy) {
    // anim
    animator_t anim = {
        .texture = LoadTexture("assets/sprites/goblin.png"),
        .anim_speed = 0.008f,
        .flipped = false,
        .state = (int)IDLE,
        .frame_count = 4,
    };
    assert(IsTextureValid(anim.texture));
    
    // path
    enemy->vision_radius = 30,
    enemy->path = (path_t) {
        .len = -1,
        .nodes = NULL,
        .target_pos = Vector2Zero(),
        .index=-1,

        .finished=false,
        .current_dir = Vector2Zero(),
        .current_speed = 0,
    };

    // entity
    enemy->entity = (entity_t) {
        .anim = anim,
        
        .bb_size = 16,
        .sprite_size = (Vector2) { 8.0f, 8.0f }, 
        .sprite_offset = (Vector2) { -1.0f, -5.0f }, 
        
        .position = (Vector2) {0.0f, 0.0f},
        .velocity = Vector2Zero(),

        .acceleration = 1000.0f,
        .friction = 0.001f,
        .speed_cap = 60.0f,
    };

    // unique functions
    enemy->set_vel_func = &pathfind_player;
    enemy->move_func = &enemy_follow_path;
    enemy->update_anim_func = &update_animation_from_path;
}

void new_path(world_t* world, enemy_t* enemy, player_t* player) {
    Vector2 enemy_tile_pos = world_to_tilemap_coord(get_entity_center(&enemy->entity));
    Vector2 player_tile_pos = world_to_tilemap_coord(get_entity_center(&player->entity));

    enemy->path = world_get_path(world, enemy_tile_pos, player_tile_pos, 200);
    if (enemy->path.len == 0) {
        enemy->path.finished = true;
    }
    
    // DEBUG:
    // printf("PATH: ");
    // for (int i = 0; i < enemy->path.len; i++) {
    //     printf("[%d, %d] ", (int)enemy->path.nodes[i].x, (int)enemy->path.nodes[i].y);
    // }
    // printf("\n");
}

void pathfind_player(world_t* world, enemy_t* enemy, player_t* player) {
    int dist = Vector2Length(Vector2Subtract(player->entity.position, enemy->entity.position)); 
    
    if (dist < 16) {
        return;
    }

    if (enemy->path.nodes) {
        float target_pos_change = Vector2Length(Vector2Subtract(get_entity_center(&player->entity), tilemap_to_world_coord(enemy->path.target_pos)));
        if (target_pos_change > MAX_TARGET_POS_CHANGE) {
            new_path(world, enemy, player);
            return;
        }
    }

    if (!enemy->path.nodes) {
        new_path(world, enemy, player);
        return;
    }
}

void update_animation_from_path(enemy_t* enemy, player_t* player) {
    // printf("FINISHED: %d, LEN: %d\n", enemy->path.finished, enemy->path.len);

    if (enemy->path.current_dir.x != 0) {
        // look in direction of path if there is horizontal movement
        enemy->entity.anim.flipped = enemy->path.current_dir.x < 0;
    }
    else {
        // when walking only vertiacally look at player
        enemy->entity.anim.flipped = player->entity.position.x < enemy->entity.position.x;
    }

    if (enemy->path.current_speed > 0) {
        play_animation(&enemy->entity.anim, ZOMBIE_ENEMY_MOVE);
    }
    else {
        play_animation(&enemy->entity.anim, ZOMBIE_ENEMY_ATTACK);
    }
}
