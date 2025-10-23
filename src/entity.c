#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/tilemap.h"
#include "../include/entity.h"
#include "../include/world.h"

Rectangle** get_col_tiles_around_entity(entity_t* entity, tilemap_t* col_map) {
    const int ts = 16; // tile size
    // tilemap at position (0,0)
    
    Rectangle** close_walls = malloc(sizeof(Rectangle*) * 3); 
    if (!close_walls) { printf("[!] malloc failed"); exit(1); }
    for (int i = 0; i < 3; i++) {
        close_walls[i] = calloc((size_t)3, sizeof(Rectangle)); 
        if (!close_walls[i]) { printf("[!] calloc failed"); exit(1); }
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

            if (collider_type == -1) {
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

void entity_move_by_velocity(entity_t* entity) {
    entity->position = Vector2Add(entity->position, Vector2Scale(entity->velocity, GetFrameTime()));
}

int entity_move_and_collide(entity_t* entity, world_t* world) {

    Rectangle** rects = get_col_tiles_around_entity(entity, world->col_map);

    int col = 0; // 0 = no col
                 // 1 = horizontal 
                 // 2 = vertical 
                 // 3 = both 

    int ts = 16; // tile size
    Vector2 offset = Vector2Scale(entity->velocity, GetFrameTime());
    
    Vector2 p_ws = {
        // entity sprite whitespace
        0.5f*(entity->bb_size - entity->sprite_size.x),
        0.5f*(entity->bb_size - entity->sprite_size.y),
    };

    // HORIZONTAL COMPONENT ----------
    entity->position.x  += offset.x;

    // check collision
    float xpos = entity->position.x;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (CheckCollisionRecs(rects[y][x], get_entity_rect(entity))) {
                // printf("Horizontal Collision \n");
                col += 1;
                // collision
                if (offset.x > 0) {
                    // collision on the right -> snap to left edge
                    entity->velocity.x = 0;
                    entity->position.x = rects[y][x].x - entity->bb_size + p_ws.x;
                } 
                else if (offset.x < 0) {
                    // collision on the left -> snap to right edge
                    entity->velocity.x = 0;
                    entity->position.x = rects[y][x].x + rects[y][x].width - p_ws.x ;
                }
            }
        }
    }

    // VERTICAL COMPONENT --------
    entity->position.y  += offset.y;

    // check collision
    float ypos = entity->position.y;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (CheckCollisionRecs(rects[y][x], get_entity_rect(entity))) {
                // printf("Vertical Collision \n");
                col += 2;
                // collision
                if (offset.y > 0) {
                    // collision on the bottom -> snap to top edge
                    entity->velocity.y = 0;
                    entity->position.y = rects[y][x].y - entity->bb_size + p_ws.y;
                } else if (offset.y <0) {
                    // collision on the top -> snap to bottom edge
                    entity->velocity.y = 0;
                    entity->position.y = rects[y][x].y + rects[y][x].height - p_ws.y;
                }
            }
        }
    }


    // Finish up
    free(rects);
    return col;
}

void entity_apply_friction(entity_t* entity) {
    float frameTime = GetFrameTime();
    
    entity->velocity = Vector2Scale(entity->velocity, pow(entity->friction, frameTime));
}

void entity_face_moving_dir(entity_t* entity) {
    if (!entity->velocity.x) { return; } 
    entity->anim.flipped = entity->velocity.x < 0;
    return;
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
