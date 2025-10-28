# pragma once

# include <stdbool.h>
# include <raylib.h>
# include <raymath.h>

typedef struct ANode {
    Vector2 parent;

    int h;
    int g;

    int f;

    bool is_collapsed;
    bool is_obstacle;
} anode_t;

typedef struct AField {
    int rows;
    int columns;
    
    anode_t** map;
} afield_t;

bool is_collider(int i);

void reset_field(afield_t *field, int** map);
afield_t generate_astar_field(int rows, int columns, int** col_map);

Vector2* get_a_star_path(afield_t *field, Vector2 start, Vector2 goal, int max_searches, int* path_length);

Vector2* do_full_a_star_search(int width, int height, int** collisions, Vector2 start, Vector2 target, int max_searches, int* path_len);

void setup_start_cell(afield_t* field, Vector2 start, Vector2 target);

Vector2 get_best_node(afield_t *field);
void collapse_node_at(afield_t *field, Vector2 node_pos);
int assign_node_as_parent(afield_t *field, Vector2 target, Vector2 node_pos);

void debug_print_field_collisions(afield_t* field);

void debug_print_field(afield_t* field);

int adjusted_euclidean(Vector2 a, Vector2 b);
