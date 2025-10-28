# pragma once

#include <raylib.h>
#include <sys/time.h>

typedef struct timeval timeval_t;
typedef struct Entity entity_t; // forward declare

// All the sprite visuals should be handeled here
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

    // never manually set, use play_animation
    int state;
    timeval_t anim_start;
} animator_t;

void render_entity(entity_t* entity);

void entity_face_moving_dir(entity_t* entity);

void play_animation(animator_t* anim, int state);

unsigned long get_time_since_ms(timeval_t begin);

Rectangle get_animation_frame(entity_t* entity);
