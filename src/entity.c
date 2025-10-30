#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/tilemap.h"
#include "../include/entity.h"
#include "../include/config.h"
#include "../include/astar.h"
#include "../include/player.h"

Rectangle** get_col_tiles_around_entity(entity_t* entity, tilemap_t* col_map) {
    const int ts = TILE_SIZE; // tile size
    // tilemap at position (0,0)
    
    Rectangle** close_walls = malloc(sizeof(Rectangle*) * 3); 
    if (!close_walls) { printf("[!] malloc failed\n"); exit(1); }
    for (int i = 0; i < 3; i++) {
        close_walls[i] = calloc((size_t)3, sizeof(Rectangle)); 
        if (!close_walls[i]) { printf("[!] calloc failed\n"); exit(1); }
    }

    Vector2 offsets[3][3] = {
        {(Vector2){-1.0f, -1.0f}, (Vector2){0.0f, -1.0f}, (Vector2){1.0f, -1.0f}},
        {(Vector2){-1.0f, 0.0f}, (Vector2){0.0f, 0.0f}, (Vector2){1.0f, 0.0f}},
        {(Vector2){-1.0f, 1.0f}, (Vector2){0.0f, 1.0f}, (Vector2){1.0f, 1.0f}},
    };

    for (int x = 0; x<3; x++) {
        for (int y = 0; y<3; y++) {
            // get entity center
            int emx = entity->position.x + (0.5*entity->bb_size);
            int emy = entity->position.y + (0.5*entity->bb_size);
            
            // calculate near tiles
            int tile_x = emx/ts + (int)offsets[y][x].x;
            int tile_y = emy/ts + (int)offsets[y][x].y;
            
            // get tile id
            int collider_type = get_tile_at(col_map, tile_x, tile_y);

            if (!is_collider(collider_type)) {
                continue; // No collision (air)
            }
            
            // get subrect by tile id 
            Rectangle sub_rect = get_collision_sub_rect(collider_type, ts);

            // move subrect to tile position
            sub_rect.x += tile_x * ts;
            sub_rect.y += tile_y * ts;
            
            // save subrect to return array
            close_walls[y][x] = sub_rect; 
        }
    }

    return close_walls;
}

Rectangle get_entity_rect(entity_t* entity) {
    int v_offset = 0.5f*(entity->bb_size - entity->sprite_size.y);
    int h_offset = 0.5f*(entity->bb_size - entity->sprite_size.x);

    return (Rectangle) {
        .x = entity->position.x + v_offset,
        .y = entity->position.y + h_offset,
        .width = entity->sprite_size.x,
        .height = entity->sprite_size.y,
    };
}

Vector2 get_entity_center(entity_t* entity) {
    Vector2 pc = entity->position;
    Vector2 offset = entity->sprite_size;
    pc = Vector2Add(pc, offset);
    return pc;
}

float get_entity_bottom(entity_t* entity) {
    return get_entity_center(entity).y + 0.5*entity->sprite_size.y;
}

void entity_face_player(entity_t* entity, player_t* player) {
    entity->anim.flipped = entity->position.x > player->entity.position.x;
}

void set_entity_center(entity_t* entity, Vector2 center_pos) {
    Vector2 offset = entity->sprite_size;
    entity->position = Vector2Subtract(center_pos, offset);
}
