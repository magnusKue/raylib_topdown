#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/tilemap.h"

tile_t* create_tile(int tile_size, int x, int y) {
    // allocate memory 
    tile_t* tile_p = malloc(sizeof(tile_t));
    if (!tile_p) { printf("[!] malloc failed"); exit(1); }
    
    // write properties
    tile_p->rect = (Rectangle) {
        .x=x,
        .y=y,
        .width=tile_size,
        .height=tile_size
    };

    // return pointer
    return tile_p;
}

void render_tilemap(void) {
    
}

