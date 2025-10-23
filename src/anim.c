#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

# include "../include/anim.h"
# include "../include/entity.h"

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

    printf("%f|%f player: \n", rect.x, rect.y);
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
