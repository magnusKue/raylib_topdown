#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/player.h"
#include "../include/world.h"
#include "../include/config.h"
#include "../include/entity.h"
#include "../include/anim.h"
#include "../include/movement.h"

void init_player(player_t* player) {
    // visual
    
    animator_t animt = {
        .texture = LoadTexture("assets/sprites/player.png"),
        .anim_speed = 0.008f,
        .flipped = false,
        .state = (int)IDLE,
        .frame_count = 4,
    };
    if (!IsTextureValid(animt.texture)) {  printf("WARINING: Player texture loading failed\n"); exit(1); }

    player->entity = (entity_t){
        .anim = animt,
        .bb_size = 16,
        .sprite_size = (Vector2) { 8, 8 }, // Bug, collision only works with some values
        .sprite_offset = (Vector2) { -1, -5 }, // X axis relative to player movement direction 

        // Physics
        .position = Vector2Zero(),
        .velocity = Vector2Zero(),
        
        .acceleration = 1000.0f,
        .friction = 0.001f,

        .speed_cap = 100.0f,
    };
}

Rectangle get_player_sprite(player_t* player, unsigned long total_ms) {

    int frame = (int)(total_ms*player->entity.anim.anim_speed) % 4;
    int size = player->entity.bb_size; 

    int rect_width = size * (player->entity.anim.flipped ? -1 : 1);
    Rectangle rect = {
        frame*size,
        (int)player->entity.anim.state * size,
        rect_width, 
        size,
    };
    return rect;
}

void player_update_velocity_by_input(player_t* player) {
    entity_t* ply_ent = &(player->entity);
    float frameTime = GetFrameTime();

    Vector2 inputs = { 0.0f, 0.0f };
    if      (IsKeyDown(KEY_A)) { inputs.x = -1; }
    else if (IsKeyDown(KEY_D)) { inputs.x =  1; }

    if      (IsKeyDown(KEY_W)) { inputs.y = -1; }
    else if (IsKeyDown(KEY_S)) { inputs.y =  1; }


    float acc_x_dt = ply_ent->acceleration * frameTime;
    Vector2 inp_x_ad = Vector2Scale(inputs, acc_x_dt);

    ply_ent->velocity = Vector2Add(ply_ent->velocity, inp_x_ad);

    // clamp velocity
    if (Vector2Length(ply_ent->velocity) < 0.01) { ply_ent->velocity = Vector2Zero(); return; }
    ply_ent->velocity.x = ply_ent->velocity.x > ply_ent->speed_cap ? ply_ent->speed_cap : ply_ent->velocity.x;
    ply_ent->velocity.x = ply_ent->velocity.x < -ply_ent->speed_cap ? -ply_ent->speed_cap : ply_ent->velocity.x;

    ply_ent->velocity.y = ply_ent->velocity.y > ply_ent->speed_cap ? ply_ent->speed_cap : ply_ent->velocity.y;
    ply_ent->velocity.y = ply_ent->velocity.y < -ply_ent->speed_cap ? -ply_ent->speed_cap : ply_ent->velocity.y;
}

void player_update_state(player_t* player) {
    if (Vector2Length(player->entity.velocity) > 10.0) {
        play_animation(&(player->entity.anim), (int)RUNNING);
    }
    else {
        play_animation(&(player->entity.anim), (int)IDLE);
    }
}

void update_player(player_t* player, world_t* world) {
    player_update_velocity_by_input(player);
    if (get_config_ptr()->player_collision) {
        entity_move_and_collide(&(player->entity), world);
    } else {
        entity_move_by_velocity(&(player->entity));
    }
    entity_face_moving_dir(&(player->entity));
    player_update_state(player);
    entity_apply_friction(&(player->entity));
}

void render_player(player_t* player, world_t* world) {
    render_entity(&player->entity, world);
}

