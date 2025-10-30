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
    enemy->vision_radius = 100,
    enemy->path = (path_t) {
        .len = -1,
        .nodes = NULL,
        .target_pos = Vector2Zero(),
        .index=-1,

        .finished=false,
        .current_dir = Vector2Zero(),
        .currently_moving = 0,
    };

    // entity
    enemy->entity = (entity_t) {
        .health = 20,

        .anim = anim,
        .action_timer = 0,
        .tint = WHITE,
        
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
    enemy->execute_state = &zombie_execute_state;
    enemy->update_state = &zombie_update_state;
}

void new_path(world_t* world, enemy_t* enemy, player_t* player) {
    Vector2 enemy_tile_pos = world_to_tilemap_coord(get_entity_center(&enemy->entity));
    Vector2 player_tile_pos = world_to_tilemap_coord(get_entity_center(&player->entity));

    enemy->path = world_get_path(world, enemy_tile_pos, player_tile_pos, 200);
    if (enemy->path.len == 0) {
        enemy->path.finished = true;
    }
}

void pathfind_player(world_t* world, enemy_t* enemy, player_t* player) {
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

void reset_action_timer(enemy_t* enemy) {
    gettimeofday(&enemy->entity.action_timer, NULL);
}

void zombie_tick_attack(enemy_t* enemy, player_t* player) {
    int attack_frequency = 500;
    long int time_since = get_time_since_ms(enemy->entity.action_timer);
    if (time_since >= attack_frequency) {
        gettimeofday(&enemy->entity.action_timer, NULL);
        hurt_player(player, 1);
    }
}

void zombie_execute_state(world_t* world, enemy_t* enemy, player_t* player) {
    // printf("path finished: %d\n", (int)enemy->path.finished);
    switch (enemy->entity.anim.state) {
        case ZOMBIE_ENEMY_IDLE:
            break;
        case ZOMBIE_ENEMY_MOVE:
            // check if a new path is needed (player moved, no path exists yet, )
            pathfind_player(world, enemy, player);
            // move to player, first by path then directly
            enemy_follow_path(enemy, player, world);
            break;
        case ZOMBIE_ENEMY_ATTACK:
            zombie_tick_attack(enemy, player);
            break;
    } 
}

void zombie_update_state(world_t* world, enemy_t* enemy, player_t* player) {
    float dist_player = Vector2Length(Vector2Subtract(player->entity.position, enemy->entity.position));

    switch (enemy->entity.anim.state) {
        case ZOMBIE_ENEMY_IDLE:
            entity_face_player(&enemy->entity, player);
            
            if (dist_player < enemy->vision_radius) {
                play_animation(&enemy->entity.anim, ZOMBIE_ENEMY_MOVE);
            }
            break;
            

        case ZOMBIE_ENEMY_MOVE:
            // --- FACE PATH OR PLAYER
            if (enemy->path.current_dir.x != 0) {
                // look in direction of path if there is horizontal movement
                enemy->entity.anim.flipped = enemy->path.current_dir.x < 0;
            }
            else {
                // when walking only vertiacally look at player
                enemy->entity.anim.flipped = player->entity.position.x < enemy->entity.position.x;
            }

            // SWITCH STATE

            if (!enemy->path.currently_moving) {
                play_animation(&enemy->entity.anim, ZOMBIE_ENEMY_ATTACK);
                reset_action_timer(enemy);
            }
            break;


        case ZOMBIE_ENEMY_ATTACK:
            entity_face_player(&enemy->entity, player);
            // if distance to player too big switch to movement
            if (dist_player > 10) {
                play_animation(&enemy->entity.anim, ZOMBIE_ENEMY_MOVE);
            }
            break;
    }

}
