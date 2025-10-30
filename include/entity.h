# pragma once 

# include <raylib.h>
# include <sys/time.h>


# include "../include/tilemap.h"
# include "../include/anim.h"

typedef struct Player player_t;

typedef struct Entity {
    int health;

    // visual
    animator_t anim;
    timeval_t action_timer;
    Color tint;

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
void entity_face_player(entity_t* entity, player_t* player);
Rectangle** get_col_tiles_around_entity(entity_t* entity, tilemap_t* col_map);
