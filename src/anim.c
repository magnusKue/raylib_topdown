#include <assert.h>
#include <stdio.h>
#include <sys/time.h>

# include "../include/anim.h"
# include "../include/entity.h"
# include "../include/player.h"
# include "../include/renderer.h"
# include "../include/world.h"
# include "../include/item.h"
# include "../include/item_meele.h"

Rectangle get_animation_frame(entity_t* entity) {
    unsigned long time_delta = get_time_since_ms(entity->anim.anim_start);

    int frame = (int)(time_delta * entity->anim.anim_speed) % entity->anim.frame_count;
    int size = entity->bb_size; 

    int rect_width = size * (entity->anim.flipped ? -1 : 1);
    Rectangle rect = {
        frame*size,
        (int)entity->anim.state * size,
        rect_width, 
        size,
    };

    return rect;
}

void play_animation(animator_t* anim, int state) {
    if (anim->state == state) { return; }
    anim->state = state;
    gettimeofday(&anim->anim_start, NULL);
}

unsigned long get_time_since_ms(timeval_t begin) {
    timeval_t now;
    gettimeofday(&now, NULL);

    unsigned long sec_diff  = now.tv_sec  - begin.tv_sec;
    long usec_diff = now.tv_usec - begin.tv_usec;

    if (usec_diff < 0) {
        usec_diff += 1000000;
        sec_diff -= 1;
    }

    unsigned long delta_ms = sec_diff * 1000 + usec_diff / 1000;
    return delta_ms;
}

void entity_face_moving_dir(entity_t* entity) {
    if (!entity->velocity.x) { return; } 
    entity->anim.flipped = entity->velocity.x < 0;
    return;
}

void render_entity(entity_t* entity, world_t* world) {
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
        .name = "Entity",
        .type = TEXTURE_PRO,
        .ypos = bottom_ypos,
        .texture = entity->anim.texture,
        .src = source_rect,
        .dest = dest_rect,
        .origin = (Vector2) { 0.0f, 0.0f },
        .rotation = 0.0f,
        .tint = entity->tint,
    });
}

item_transl_t get_item_animation_frame(item_t* item, player_t* player) {
    itemdata_meele_t* item_data = get_meele_data(item->item_data);
    
    item_transl_t translation = { Vector2Zero(), .0f };
    
    long int anim_time = get_time_since_ms(player->last_item_action);
    
    if (anim_time >= item_data->animation_length_ms) { return translation; };

    double time_fraction = ((float)anim_time / item_data->animation_length_ms);
    double current_index = item_data->used_frames * time_fraction;
    int index_int = floor(current_index);
    double frac = current_index - index_int;
    // printf("INDEX: %d TIME: %ld FRAME_T: %lf\n", index_int, anim_time, frac);
    
    if (index_int >= item_data->used_frames - 1) {
        translation = item_data->animation[index_int];
    }
    else {
        // lerp
        translation = lerp_item_transl(item_data->animation[index_int], item_data->animation[index_int+1], frac);
    }
        // no lerping for last element

    return translation;
}
