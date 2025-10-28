#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/astar.h"


bool is_collider(int i) {
	if (i == -1 || i == 10) {
		return false;
	}
	else {
		return true;
	}
}

void reset_field(afield_t *field, int** map) {
	for (int r = 0; r < field->rows; r++) {
		for (int c = 0; c < field->columns; c++) {
			bool is_obstacle = is_collider(map[r][c]);

			field->map[r][c] = (anode_t){
				.parent = {-1, -1},
				.h = -1,
				.g = -1,
				.f = -1,
				.is_collapsed = false,
				.is_obstacle = is_obstacle,
			};
		}
	}
}

afield_t generate_astar_field(int rows, int columns, int** col_map) {
	anode_t **map = calloc(sizeof(anode_t *), rows);
	assert(map);
	for (int r = 0; r < rows; r++) {
		map[r] = (anode_t *)calloc(sizeof(anode_t), columns);
		assert(map[r]);
	}

	afield_t field = {
		rows = rows,
		columns = columns,
		map = map,
	};

	reset_field(&field, col_map);

	return field;
}

void free_field(afield_t* field) {
    if (!field || !field->map)
        return;

    for (int i = 0; i < field->rows; i++) {
        free(field->map[i]);
    }
    free(field->map);
    field->map = NULL;
}

int adjusted_euclidean(Vector2 a, Vector2 b) {
	int dx = b.x - a.x;
	int dy = b.y - a.y;
	return (int)round(sqrt(dx * dx + dy * dy) * 10.0);
}

int octile_heuristic(Vector2 a, Vector2 b) {
    int dx = abs((int)b.x - (int)a.x);
    int dy = abs((int)b.y - (int)a.y);
    int min_d = dx < dy ? dx : dy;
    int max_d = dx > dy ? dx : dy;
    return 14 * min_d + 10 * (max_d - min_d);
}

void update_node_values(afield_t *field, Vector2 target, Vector2 node_pos, Vector2 parent_pos) {
	anode_t *node = &field->map[(int)node_pos.y][(int)node_pos.x];
	
	int traversal_cost = (node_pos.x == parent_pos.x) || (node_pos.y == parent_pos.y) ? 10 : 14;
	int new_g = field->map[(int)parent_pos.y][(int)parent_pos.x].g + traversal_cost;

	if (node->parent.y != -1) {
		// new potential cost
		if (node->g <= new_g) {
			return;
		}
	}
		
	// set parent
	node->parent = parent_pos;
		
	// set values
	node->g = new_g;
	node->h = octile_heuristic(node_pos, target);
	node->f = node->g + node->h;

}

bool is_valid_child(afield_t* field, Vector2 parent, Vector2 child) {
	int child_x = child.x;
	int child_y = child.y;

	// if in bounds set note as parent
	if (child_x < 0 || child_y < 0 || child_x >= field->columns || child_y >= field->rows) {
		// printf("out of bounds node at: %d %d\n", child_x, child_y);
		return false;
	}

	if (field->map[child_y][child_x].is_collapsed) { return false; }

	if (field->map[child_y][child_x].is_obstacle) { return false; }


	// dont allow diagonal passing trough obstacles
	
	if (child.x == parent.x || child.y == parent.y) {
		// no diagonal movement
		return true;
	}
	
	Vector2 dir = Vector2Subtract(child, parent);
	// printf("Direction: %i %i\n", (int)dir.x, (int)dir.y);

	bool is_x_obstacle = field->map[(int)(parent.y+dir.y)][(int)parent.x].is_obstacle;
	bool is_y_obstacle = field->map[(int)parent.y][(int)(parent.x+dir.x)].is_obstacle;

	// printf("obstacles: %i %i\n", is_x_obstacle, is_y_obstacle);
	if (is_x_obstacle || is_y_obstacle) {
		return false;
	}

	return true;
}

int assign_node_as_parent(afield_t* field, Vector2 target, Vector2 node_pos) {
	Vector2 offsets[3][3] = {
		{(Vector2){-1.0f, -1.0f}, (Vector2){0.0f, -1.0f}, (Vector2){1.0f, -1.0f}},
		{(Vector2){-1.0f, 0.0f}, (Vector2){0.0f, 0.0f}, (Vector2){1.0f, 0.0f}},
		{(Vector2){-1.0f, 1.0f}, (Vector2){0.0f, 1.0f}, (Vector2){1.0f, 1.0f}},
	};

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
            if (x == 1 && y == 1) { continue; } // skip parent

			int child_x = node_pos.x + offsets[y][x].x;
			int child_y = node_pos.y + offsets[y][x].y;

			if (!is_valid_child(field, node_pos, (Vector2) { child_x, child_y } )) {
				continue;
			}
			
			if (child_x == (int)target.x && child_y == (int)target.y) { 
				// target found!!
				field->map[(int)target.y][(int)target.x].is_collapsed = true;
				field->map[(int)target.y][(int)target.x].parent = node_pos;
				return 1;
			} 

			update_node_values(field, target, (Vector2) {child_x, child_y}, node_pos);
		}
	}
	return 0;
}

void collapse_node_at(afield_t *field, Vector2 node_pos) {
	field->map[(int)node_pos.y][(int)node_pos.x].is_collapsed = true;
}

Vector2 get_best_node(afield_t *field) {
	Vector2 best_pos = {-1, -1};
	int best_f = __INT_MAX__;
	int best_h = __INT_MAX__;

	for (int y = 0; y < field->rows; y++) {
		for (int x = 0; x < field->columns; x++) {
			anode_t *test_node = &field->map[y][x];
			
			if (test_node->f == -1) { continue; }

			// TODO what if two nodes have same f and same heuristic? (symetric
			// problem)
			if ((
                    (test_node->f < best_f) ||
                    (test_node->f == best_f && test_node->h < best_h)
                 ) &&
                    !test_node->is_collapsed
				   && 
				   	!test_node->is_obstacle
				 ) {
				// new best
				best_pos = (Vector2){x, y};
				best_f = test_node->f;
				best_h = test_node->h;
			}
		}
	}

	// will return -1 -1 when no uncollapsed node was found
	return best_pos;
}

Vector2* reconstruct_path(afield_t* field, Vector2 start, Vector2 end, int *out_length) {
	// entirely chatgpt i had no motivation left
	int rows = field->rows;
	int columns = field->columns;
	anode_t** map = field->map; 

    int max_len = rows * columns;  // safe upper bound
    Vector2 *path = malloc(max_len * sizeof(Vector2));
    if (!path) return NULL;

    int count = 0;
    Vector2 current = end;

    while (!(current.x == start.x && current.y == start.y)) {
        path[count++] = current;

        int px = (int)current.x;
        int py = (int)current.y;

        // bounds check
        if (px < 0 || px >= columns || py < 0 || py >= rows) break;

        current = map[py][px].parent;

        if (count >= max_len) break;  // prevent infinite loops
    }

    path[count++] = start;

    // reverse path so it goes start -> end
    for (int i = 0; i < count / 2; i++) {
        Vector2 tmp = path[i];
        path[i] = path[count - 1 - i];
        path[count - 1 - i] = tmp;
    }

    *out_length = count;
    return path;
}

Vector2* get_a_star_path(afield_t *field, Vector2 start, Vector2 goal, int max_searches, int* path_length) {
    setup_start_cell(field, start, goal);
    collapse_node_at(field, start);
    
    assign_node_as_parent(field, goal, start);
	// debug_print_field(field);
    
    for (int x = 0; x < max_searches; x++) {
        Vector2 best = get_best_node(field);
        if (best.x == -1 || best.y == -1) {
            // all search paths exausted
            // debug_print_field(field);
            // printf("TARGET NOT REACHABLE!\n");

			*path_length = 0;
            return NULL;
        }
        // printf("Best node: %d %d\n", (int)best.x, (int)best.y);

        int found = assign_node_as_parent(field, goal, best);
        collapse_node_at(field, best);
        // printf("collapsed node at: %d %d", (int)best.x, (int)best.y);

        if (found) {
            // debug_print_field(field);
            // printf("FOUND TARGET!\n");
            return reconstruct_path(field, start, goal, path_length);
		}
    }

    // printf("All searches wasted, target not found, quitting!\n");
	*path_length = 0;
	return NULL;
}

Vector2* do_full_a_star_search(int width, int height, int** collisions, Vector2 start, Vector2 target, int max_searches, int* path_len) {
	afield_t field = generate_astar_field(height, width, collisions);
	
	Vector2* path = get_a_star_path(&field, start, target, max_searches, path_len);

	free_field(&field);
	return path;
}

void setup_start_cell(afield_t* field, Vector2 start, Vector2 target) {
	int heuristic = octile_heuristic(start, target);

	field->map[(int)start.y][(int)start.x].g = 0;
	field->map[(int)start.y][(int)start.x].h = heuristic;
	field->map[(int)start.y][(int)start.x].f = heuristic;
}

void debug_print_field_collisions(afield_t* field) {
	printf("\nAstar Collision:\n");
	for (int r = 0; r < field->rows; r++) {
		for (int c = 0; c < field->columns; c++) {
			printf("%i", field->map[r][c].is_obstacle);
		}
		printf("\n");
	}
	printf("\n");
}

void debug_print_field(afield_t* field) {
	char* arrows[3][3] = {
		{"↖", "↑", "↗"},
		{"←", "•", "→"},
		{"↙", "↓", "↘"}
	};

	printf("\nAstar Field:\n");
	for (int r = 0; r < field->rows; r++) {
		for (int c = 0; c < field->columns; c++) {
			printf(" |");
			if (field->map[r][c].is_obstacle) {
				printf("#");
			}
			// else if (field->map[r][c].is_collapsed) {
			//     printf("[]");
			// }
			else {
				Vector2 parent = field->map[r][c].parent;
				if (parent.x != -1 && field->map[r][c].is_collapsed) {
					Vector2 par_offs = Vector2Subtract(parent, (Vector2) {c, r});
					printf("%s", arrows[(int)par_offs.y+1][(int)par_offs.x+1]);
				}
				else {
					printf(" ");
				}
			}
			printf("| ");
		}
		printf("\n");
	}
	printf("\n");

}
