#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "../include/player.h"
#include "../include/world.h"

#define SP_WIDTH 16

void init_player(player_t* player) {
    // visual
    player->state = IDLE;
    player->anim_speed = 0.008f;
    player->flipped = false;

    player->texture = LoadTexture("assets/sprites/player.png");

    if (!IsTextureValid(player->texture)) {  printf("WARINING: Player texture loading failed"); return; }

    // physics
    player->position.x = 0.0f;
    player->position.y = 0.0f;

    player->velocity.x = 0.0f;
    player->velocity.y = 0.0f;
    
    player->acceleration = 1000.0f;
    player->friction = 0.001f;

    player->speed_cap = 100.0f;
}


Rectangle get_player_sprite(player_t* player, unsigned long total_ms) {
    int frame = (int)(total_ms*player->anim_speed) % 4;
    int size = SP_WIDTH; 
    
    int rect_width = size * (player->flipped ? -1 : 1);
    Rectangle rect = {
        frame*size,
        (int)player->state * size,
        rect_width, 
        size,
    };
    return rect;
}

void render_player(player_t* player, unsigned long total_ms) {
    Rectangle dest = {
        player->position.x,
        player->position.y,
        SP_WIDTH,
        SP_WIDTH,
    };
    DrawTexturePro(player->texture, get_player_sprite(player, total_ms), dest, (Vector2) { 0.0, 0.0 }, 0, WHITE);
}


void player_update_velocity_by_input(player_t* player) {
    float frameTime = GetFrameTime();

    Vector2 inputs = { 0.0f, 0.0f };
    if      (IsKeyDown(KEY_A)) { inputs.x = -1; }
    else if (IsKeyDown(KEY_D)) { inputs.x =  1; }

    if      (IsKeyDown(KEY_W)) { inputs.y = -1; }
    else if (IsKeyDown(KEY_S)) { inputs.y =  1; }


    // player->velocity = player->velocity + (inputs * player->acceleration * deltaTime);
    float acc_x_dt = player->acceleration * frameTime;
    Vector2 inp_x_ad = Vector2Scale(inputs, acc_x_dt);

    player->velocity = Vector2Add(player->velocity, inp_x_ad);

    // clamp velocity
    if (Vector2Length(player->velocity) < 0.01) { player->velocity = Vector2Zero(); return; }
    player->velocity.x = player->velocity.x > player->speed_cap ? player->speed_cap : player->velocity.x;
    player->velocity.x = player->velocity.x < -player->speed_cap ? -player->speed_cap : player->velocity.x;

    player->velocity.y = player->velocity.y > player->speed_cap ? player->speed_cap : player->velocity.y;
    player->velocity.y = player->velocity.y < -player->speed_cap ? -player->speed_cap : player->velocity.y;
}

void player_move_by_velocity(player_t* player) {
    player->position = Vector2Add(player->position, Vector2Scale(player->velocity, GetFrameTime()));
}

void player_apply_friction(player_t* player) {
    float frameTime = GetFrameTime();
    player->velocity = Vector2Scale(player->velocity, pow(player->friction, frameTime));
    // player->velocity = Vector2Scale(player->velocity, player->friction);
}


void player_face_moving_dir(player_t* player) {
    if (!player->velocity.x) { return; } 
    player->flipped = player->velocity.x < 0;
    return;
}

void player_update_state(player_t* player) {
    if (Vector2Length(player->velocity) > 10.0) {
        player->state = RUNNING;
    }
    else {
        player->state = IDLE;
    }
}

void update_player(player_t* player, world_t* world) {
    // printf("PLAYER POS: %f %f\n", player->position.x, player->position.y);
    // printf("PLAYER VEL: %f %f\n", player->velocity.x, player->velocity.y);

    player_update_velocity_by_input(player);
    player_move_by_velocity(player);
    player_face_moving_dir(player);
    player_update_state(player);
    player_apply_friction(player);
}
