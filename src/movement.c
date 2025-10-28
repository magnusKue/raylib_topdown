#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/world.h"
#include "../include/config.h"

#include "../include/movement.h"

#include "../include/entity.h"
#include "../include/player.h"
#include "../include/enemy.h"


int enemy_follow_path(enemy_t* enemy, player_t* player, world_t* world) {
    if (!enemy->path.nodes || enemy->path.len < 1 || enemy->path.index == -1 || enemy->path.finished) {
        // no path calcutated
        return 1;
    }

    if (enemy->path.finished) {
        printf("TARGET REACHED!\n");
        // GOAL WAS ALREADY REACHED
        // chase player
        return 0;
    }
    
    Vector2 target_node = enemy->path.nodes[enemy->path.index];
    Vector2 dir = Vector2Subtract(target_node, get_entity_center(&enemy->entity));
    float dir_len = Vector2Length(dir);
    Vector2 norm_dir = Vector2Normalize(dir);

    printf("Next node posi [%d|%d]\n", (int)dir.x, (int)dir.y);
    printf("Next direction [%f|%f]\n", norm_dir.x, norm_dir.y);
    
    float speed = enemy->entity.speed_cap * 0.7;
    Vector2 step = Vector2Scale(norm_dir, GetFrameTime() * speed);
    float step_len = Vector2Length(step);
    printf("Next step into [%f|%f]\n", step.x, step.y);

    if (step_len < dir_len) {
        Vector2 new_pos = Vector2Add(get_entity_center(&enemy->entity), step);
        set_entity_center(&enemy->entity, new_pos);
        // DONE
        return 0;
    }

    // ELSE: NODE REACHED
    set_entity_center(&enemy->entity, target_node);
    if (enemy->path.index == enemy->path.len -1) {
        enemy->path.finished = true;
        // GOAL REACHED
        return 0;
    }
    enemy->path.index++;

    return 0;
}

void entity_move_by_velocity(entity_t* entity) {
    entity->position = Vector2Add(entity->position, Vector2Scale(entity->velocity, GetFrameTime()));
}

void entity_apply_friction(entity_t* entity) {
    float frameTime = GetFrameTime();
    
    entity->velocity = Vector2Scale(entity->velocity, pow(entity->friction, frameTime));
}

int entity_move_and_collide(entity_t* entity, world_t* world) {

    Rectangle** rects = get_col_tiles_around_entity(entity, world->col_map);

    int col = 0; // 0 = no col
                 // 1 = horizontal 
                 // 2 = vertical 
                 // 3 = both 

    int ts = TILE_SIZE; // tile size
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
