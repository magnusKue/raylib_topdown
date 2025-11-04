# pragma once

#include <raylib.h>

# include "../include/anim.h"

typedef struct World world_t;
typedef struct Player player_t;

typedef struct ItemTransl {
    Vector2 position;
    float rotation;
} item_transl_t;

typedef enum ItemType {
    MEELE_WEAPON,
} itemtype_t;

typedef struct Item item_t;
typedef struct Item {
    Texture2D sprite;
    Vector2 render_offset;
    Vector2 rot_origin_offs;

    itemtype_t type;
    int cooldown_ms;

    void* item_data;
    void (*item_action)(player_t*, world_t*);
    item_transl_t (*get_item_anim_transl)(item_t*, player_t*);
} item_t;

item_transl_t lerp_item_transl(item_transl_t a, item_transl_t b, float factor);

void free_item(item_t* item);
void do_item_action(player_t* player, world_t* world);
