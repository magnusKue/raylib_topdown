#pragma once

#include <raylib.h>

typedef struct Tile {
    // stores the rect of its texture in the tileset texture
    Rectangle rect;
} tile_t;

typedef struct Tileset {
    // stores list of tiles and their texture 
    tile_t* tiles;
    int num_tiles;

    int tile_size;

    Texture2D tilesheet;
} tileset_t;

typedef struct TilesetManager {
    // List of Tilesets
    tileset_t* tilesets;
    int num_tilesets;
} tileset_manager_t;


tile_t* create_tile(int tile_size, int x, int y);

void render_tilemap(void);
