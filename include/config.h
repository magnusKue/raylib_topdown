# pragma once

# include <stdbool.h>


# define TILE_SIZE 16

typedef struct Config {
    bool render_colliders;
    bool render_paths;
    bool player_collision;

    // layers
    bool render_ground;
    bool render_objects;
    bool render_tips;
} config_t;

extern config_t global_config;

config_t* get_config_ptr(void);
