#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/tilemap.h"

tile_t* create_tile(int tile_size, int x, int y) {
    // allocate memory 
    tile_t* tile_p = (tile_t*)malloc(sizeof(tile_t));
    if (!tile_p) { printf("[!] malloc failed"); exit(1); }
    
    // write properties
    tile_p->rect = (Rectangle) {
        .x=x,
        .y=y,
        .width=tile_size,
        .height=tile_size
    };

    printf("created tile at (%d|%d) with size (%d)\n", x, y, tile_size);

    // return pointer
    return tile_p;
}

tileset_t* load_tileset(int tile_size, char* tile_sheet_path) {
    assert(tile_size > 0);

    tileset_t* tileset_p = malloc(sizeof(tileset_t));
    if (!tileset_p) { printf("[!] malloc failed"); exit(1); }

    Texture2D texture = LoadTexture(tile_sheet_path);
    assert(IsTextureValid(texture));
    tileset_p->tilesheet = texture; 

    int rows = tileset_p->tilesheet.height / tile_size;
    int columns = tileset_p->tilesheet.width / tile_size;
    int total_tiles = rows * columns;

    tileset_p->num_tiles = total_tiles;
    tileset_p->tile_size = tile_size; 

    tileset_p->tiles = malloc(sizeof(tile_t*) * total_tiles);
    if (!tileset_p->tiles) { printf("[!] malloc failed"); exit(1); }
    
    // populate tile list
    int index = 0;
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            tileset_p->tiles[index] = (tile_t*)create_tile(tile_size, x * tile_size, y * tile_size);  
            printf("POSITION: (%f|%f)\n\n", tileset_p->tiles[index]->rect.x, tileset_p->tiles[index]->rect.y);

            index++;
        }
    }

    printf("Tileset with (%d) tiles created!\n", total_tiles);

    return tileset_p;
}


void render_tilemap(void) {
    
}

