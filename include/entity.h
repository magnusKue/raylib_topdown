# pragma once 

# include <raylib.h>
# include <raylib.h>

# include "../include/tilemap.h"
# include "../include/anim.h"

typedef struct Entity {
    // visual
    animator_t anim;

    int bb_size; // bounding box
    Vector2 sprite_size;
    Vector2 sprite_offset;

    // Physics
    Vector2 position;
    Vector2 velocity;

    float acceleration;
    float friction;
    float speed_cap;
} entity_t;


void set_entity_center(entity_t* entity, Vector2 center_pos);
Vector2 get_entity_center(entity_t* entity);
float get_entity_bottom(entity_t* entity);
Rectangle get_entity_rect(entity_t* entity);
Rectangle** get_col_tiles_around_entity(entity_t* entity, tilemap_t* col_map);
