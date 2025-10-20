#include "../include/tilemap.h"
#include "../include/csvparser.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

tilemap_t* csv_to_tilemap(char* csv) {
    int r, c; // number of rows and columns
    count_rows_columns(csv, &r, &c);
    
    // allocate memory
    int** array = allocate_2d_array(r, c);

    // read csv into array
    read_csv_into_array(csv, array, c);

    // print_array2d(array, r, c);
    
    // Create, populate and return tilemap object
    tilemap_t* tm = malloc(sizeof(tilemap_t));
    tm->tmap = array;
    tm->rows = r;
    tm->columns = c;
    return tm;
}

void print_array2d(int** array, int rows, int columns) {
    printf("\n\nPARSED DATA:\n  ");

    for (int r = 0; r < rows; r++) {
        for (int x = 0; x < columns; x++) {
            printf("%d ", array[r][x]);
        }
        if (!(r == rows-1)) {
            printf(" \n  ");
        }
    }
    printf("\nROW: [%d] COL: [%d]\n", rows, columns);

}

void read_csv_into_array(char* csv, int** array, int columns) {
    char* text = strdup(csv);

    const char* delimiters = ",\n"; 

    char* token;

    token = strtok(text, delimiters);

    int index = 0;
    int row = 0;
    int column = 0;

    while (token != NULL) {
        // calculate 2d position from token index
        row = index / columns;
        column = index % columns;

        // populate 2d array
        int value = (int)strtol(token, NULL, 10); // base 10
        array[row][column] = value;

        // next token
        index++;
        token = strtok(NULL, delimiters);
    }
    free(text);
}

int** allocate_2d_array(int max_row, int max_column) {
    // ALLOCATE BASE ARRAY
    int** array_2d = calloc((size_t)max_row, sizeof(int*));
    if (!array_2d) { printf("[!] malloc failed"); exit(1); }

    // ALLOCATE EACH ROW ARRAY
    for (int r = 0; r < max_row; r++) {
        array_2d[r] = (int*)calloc(max_column, sizeof(int));
        if (!array_2d[r]) { printf("[!] malloc failed"); exit(1); }
    }

    return array_2d;
}

void count_rows_columns(char* csv, int* num_rows, int* num_columns) {
    char* text = strdup(csv);

    const char* newline = "\n";  
    char* token;

    token = strtok(text, newline);

    // Walk through other tokens
    int row = 0;
    int columns = 0;
    while (token != NULL) {
        if (!row) { 
            columns = count_chars(',', token) + 1;
        }
        row++;
        token = strtok(NULL, newline);
    }
    free(text);

    *num_rows = row;
    *num_columns = columns;    
}

int count_chars(char c, char* input_str) {
    char* str = input_str;
    int count = 0;
    
    while (*str != '\0') {
        if (*str == c) {
            count++;
        }
        str++; 
    }
    
    return count;
}
