# pragma once

# include <raylib.h>

#include "../include/item.h"

typedef struct ItemDataMeele {
    int damage;

    item_transl_t animation[10];
    int used_frames;
    int animation_length_ms;
    
    Rectangle hurtbox;
    float attack_speed;
} itemdata_meele_t;

itemdata_meele_t* get_meele_data(void* data);

item_t* create_meele_item_base();

void meele_item_attack(player_t* player, world_t* world);
