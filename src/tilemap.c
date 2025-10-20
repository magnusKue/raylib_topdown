#include <raylib.h>

#include <string.h>
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

tilemap_t* load_tilemap(char* map_path) {
    char* raw_file = LoadFileText(map_path);
    char* text = strdup(raw_file);

    const char* delimiter = ",";  
    
    // FIRST PASS (calc token amount)

    char *token_c;

    // Get the first token
    token_c = strtok(text, delimiter);

    // Walk through other tokens
    int max_row = 0;
    int column = 0;
    int max_column = 0;
    while (token_c != NULL) {
        if (token_c[0]=='\n') {
            max_row++;
            max_column = column > max_column ? column : max_column;
            column=0;
        }
        else {
            column++;
        }
        token_c = strtok(NULL, delimiter);
    }
    
    printf("NUM_COLMN: [%d]\n", max_column);
    printf("NUM_ROWS : [%d]\n", max_row);

    // ALLOCATE MEMORY
    int** map_array = calloc((size_t)max_row, sizeof(int*));
    if (!map_array) { printf("[!] malloc failed"); exit(1); }
    for (int r = 0; r < max_row; r++) {
        map_array[r] = (int*)calloc(max_column, sizeof(int));
        if (!map_array[r]) { printf("[!] malloc failed"); exit(1); }
    }

    // SECOND PASS (parse tokens)
    char* text_2 = strdup(raw_file);

    char *token;

    // Get the first token
    token = strtok(text_2, delimiter);

    // Walk through other tokens
    int row_i = 0;
    int column_i = 0;

    while (token != NULL) {
        if (token[0]=='\n') {
            // printf("LINE (%d)\n", row_i);
            row_i++;
            column_i=0;
        }
        else {
            char *endptr;
            int num = (int)strtol(token, &endptr, 10); // base 10

            if (endptr == token) {
                // printf("ERROR");
            } else {
                if (column_i < max_column) { 
                    map_array[row_i][column_i] = num;
                    // printf("(%d)'%d', ", column_i, num);
                }
            }

            column_i++;
        }

        token = strtok(NULL, delimiter);
    }
    
    // print result
    printf("\n\nPARSED DATA:\n\n");
    for (int r = 0; r < max_row; r++) {
        for (int x = 0; x < max_column; x++) {
            printf("%d ", map_array[r][x]);
        }
        printf("\n");
    }

    // create tilemap
    tilemap_t* tm = malloc(sizeof(tilemap_t));
    tm->tmap = map_array;
    tm->rows = max_row;
    tm->columns = max_column;

    return tm;
}

void render_tile(int index, tileset_t* ts, int x, int y) {
    // Air
    if (index == -1) { return; }

    // Solids
    DrawTexturePro(
        ts->tilesheet, 
        /*SRC*/ts->tiles[index]->rect, 
        /*DEST*/(Rectangle) {
            x,
            y,
            ts->tile_size,
            ts->tile_size,
        }, 
        /*ORIGIN*/(Vector2) { 0.0f, 0.0f }, 
        0.0f, WHITE
    );
}

void render_tileset(tileset_t* ts) {
    for (int x = 0; x < ts->num_tiles; x++) {
        render_tile(x, ts, ts->tiles[x]->rect.x, ts->tiles[x]->rect.y);
    }
}


void render_tilemap(tilemap_t* tmap, tileset_t* tset) {
    for (int r = 0; r < tmap->rows; r++) {
        for (int x = 0; x < tmap->columns; x++) {
            render_tile((int)tmap->tmap[r][x], tset, x*16, r*16);
        }
    }
}
