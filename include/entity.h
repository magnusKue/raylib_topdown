# pragma once 

# include <raylib.h>

# include "../include/world.h"
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

int entity_move_and_collide(entity_t* entity, world_t* world);

void entity_move_by_velocity(entity_t* entity);
void entity_apply_friction(entity_t* entity);
void entity_face_moving_dir(entity_t* entity);

Rectangle get_entity_rect(entity_t* entity);
Rectangle** get_col_tiles_around_entity(entity_t* entity, tilemap_t* col_map);
