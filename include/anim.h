# pragma once

#include <raylib.h>
#include <sys/time.h>

typedef struct timeval timeval_t;
typedef struct Entity entity_t; // forward declare

// Basic animation system.
// All tracks must have the same number of frames 
// and the same animation speed
// state (current anim) is the y offset into the sprite sheet
// For ease of use pass state as enum


typedef struct Animator {
    Texture2D texture;
    
    float anim_speed;
    
    int flipped;

    int frame_count;
    int state;
    timeval_t anim_start;
} animator_t;

void play_animation(animator_t* anim, int state);

unsigned long get_time_since_ms(timeval_t begin);

Rectangle get_animation_frame(entity_t* entity);

void render_animtor(entity_t* entity);
