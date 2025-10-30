#include <raylib.h>
#include <assert.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/player.h"
#include "../include/world.h"
#include "../include/config.h"
#include "../include/entity.h"
#include "../include/anim.h"
#include "../include/movement.h"
#include "../include/renderer.h"

void init_player(player_t* player) {

    player->current_item = NULL;
    player->no_arms_tex = LoadTexture("assets/sprites/player_no_hand.png");
    assert(IsTextureValid(player->no_arms_tex));


    // visual
    animator_t animt = {
        .texture = LoadTexture("assets/sprites/player.png"),
        .anim_speed = 0.008f,
        .flipped = false,
        .state = (int)IDLE,
        .frame_count = 4,
    };
    if (!IsTextureValid(animt.texture)) {  printf("WARINING: Player texture loading failed\n"); exit(1); }
    
    gettimeofday(&player->last_damage, NULL);
    player->last_damage.tv_usec = 0;
    player->last_damage.tv_sec = 0;
    
    player->entity = (entity_t){
        .health = 20,

        .anim = animt,
        .bb_size = 16,
        .sprite_size = (Vector2) { 8, 8 }, // Bug, collision only works with some values
        .sprite_offset = (Vector2) { -1, -5 }, // X axis relative to player movement direction 

        .action_timer = 0,
        // Physics
        .position = Vector2Zero(),
        .velocity = Vector2Zero(),
        
        .acceleration = 1000.0f,
        .friction = 0.001f,

        .speed_cap = 100.0f,
    };
}

void hurt_player(player_t* player, int damage) {
    player->entity.health -= damage;
    gettimeofday(&player->last_damage, NULL);

    printf("Player took damage, Health: %d\n", player->entity.health);

    if (player->entity.health <= 0) {
        printf("We died :(\n");
        player->entity.health = 10;
    }
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

void equip_item(player_t* player, item_t* item) {
    player->current_item = item;  
}

void unequip_item(player_t* player) {
    UnloadTexture(player->current_item->sprite);
    free_item(player->current_item);
    player->current_item = NULL;  
}

void render_player_item(player_t* player) {
    item_t* item = player->current_item;
    
    int what_the_flip = player->entity.anim.flipped ? -1 : 1;
    Vector2 item_size = { item->sprite.width, item->sprite.height };

    Vector2 flip_rend_offs = item->render_offset;
    flip_rend_offs.x *= what_the_flip;

    Rectangle src = { 0, 0, item_size.x * what_the_flip, item_size.y };
    Rectangle dest = { 
        player->entity.position.x + flip_rend_offs.x, 
        player->entity.position.y + flip_rend_offs.y, 
        item_size.x, item_size.y 
    };

    float player_ypos = get_entity_bottom(&player->entity);
    push_to_render_buffer((renderdata_t) {
        .name = "sword",
        .type = TEXTURE_PRO,
        .ypos = player_ypos - 0.001,
        .texture = item->sprite,
        .src = src,
        .dest = dest,
        .origin = (Vector2) { 0.0f, 0.0f },
        .rotation = 0.0f,
        .tint = WHITE,
    });
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
    Color tint = get_time_since_ms(player->last_damage) < 300 ? RED : WHITE;
    player->entity.tint = tint;
    
    if (player->current_item) {
        // RENDER ITEM
        render_player_item(player);
    }

    // Render player
    
    entity_t* entity = &player->entity;
    Vector2 offset = {
        .x = entity->sprite_offset.x * (entity->anim.flipped?-1:1),
        .y = entity->sprite_offset.y,
    };

    Rectangle dest_rect = {
        entity->position.x + offset.x,
        entity->position.y + offset.y,
        entity->bb_size,
        entity->bb_size,
    };

    Rectangle source_rect = get_animation_frame(entity);
    
    float bottom_ypos = get_entity_bottom(entity);

    push_to_render_buffer((renderdata_t) {
        .name = "player",
        .type = TEXTURE_PRO,
        .ypos = bottom_ypos,
        .texture = player->current_item ? player->no_arms_tex : entity->anim.texture,
        .src = source_rect,
        .dest = dest_rect,
        .origin = (Vector2) { 0.0f, 0.0f },
        .rotation = 0.0f,
        .tint = entity->tint,
    });

}

