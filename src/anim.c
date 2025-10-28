#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

# include "../include/anim.h"
# include "../include/entity.h"
# include "../include/config.h"

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

    // printf("%f|%f player: \n", rect.x, rect.y);
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

void render_entity(entity_t* entity) {
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

    DrawTexturePro(entity->anim.texture, source_rect, dest_rect, (Vector2) { 0.0, 0.0 }, 0, WHITE);


    if (get_config_ptr()->render_colliders) {
        Vector2 center = get_entity_center(entity);
        DrawCircleV(center, 2.0, BLUE);
    }

}
