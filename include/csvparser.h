# pragma once

tilemap_t* csv_to_tilemap(char* csv);

int count_chars(char c, char* input_str);
int** allocate_2d_array(int max_row, int max_column);

void read_csv_into_array(char* csv, int** array, int columns);
void count_rows_columns(char* csv, int* num_rows, int* num_columns);
void print_array2d(int** array, int rows, int columns);
