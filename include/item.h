# pragma once

#include <raylib.h>

typedef struct Player player_t;
typedef struct World world_t;

typedef enum ItemType {
    MEELE_WEAPON,
} itemtype_t;

typedef struct Item {
    Texture2D sprite;
    Vector2 render_offset;

    itemtype_t type;

    void* item_data;
    void (*item_action)(player_t*, world_t*);
} item_t;

void free_item(item_t* item);
