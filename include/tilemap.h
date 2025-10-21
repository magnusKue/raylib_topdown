# pragma once

#include <raylib.h>

typedef struct Tile {
    // stores the rect of its texture in the tileset texture
    Rectangle rect;
} tile_t;

typedef struct Tileset {
    // stores list of tile pointers and their shared texture
    // index of tiles is their position in the tile sheet
    tile_t** tiles;
    int num_tiles;

    int tile_size;

    Texture2D tilesheet;
} tileset_t;

typedef struct Tilemap {
    // stores an 2D array of the tile map
    int** tmap;
    int columns;
    int rows;
} tilemap_t;


tile_t* create_tile(int tile_size, int x, int y);
tilemap_t* load_tilemap(char* map_path);
tileset_t* load_tileset(int tile_size, char* tile_sheet_path);

void render_tile(int index, tileset_t* ts, int x, int y);
void render_tileset(tileset_t* ts);
void render_tilemap(tilemap_t* tmap, tileset_t* tset);
void render_collision_tiles(Rectangle** tile_rects);

int get_tile_at(tilemap_t* map, int x, int y);

Rectangle get_collision_sub_rect(int tiletype, int bb);


