#pragma once

#include <raylib.h>


typedef struct TilesetManager {
    tileset_t* tilesets;
    int num_tilesets;
} tileset_manager_t;

typedef struct Tileset {
    tile_t* tiles;
    int num_tiles;
} tileset_t;

typedef struct Tile {
    Texture2D texture;
} tile_t;

tile_t* slice_tileset(Image2D spritesheet, int width, int height);

void render_tilemap(void);
