#include "../include/tilemap.h"
#include "../include/csvparser.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

tilemap_t* csv_to_tilemap(char* csv) {
    char* text = strdup(csv);

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
    char* text_2 = strdup(csv);

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
    
    tilemap_t* tm = malloc(sizeof(tilemap_t));
    tm->tmap = map_array;
    tm->rows = max_row;
    tm->columns = max_column;
    return tm;
}
