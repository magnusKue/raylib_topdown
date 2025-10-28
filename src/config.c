#include "../include/config.h"

// Initialize debug_mode to false or true as needed
config_t global_config = {
    // DEBUG
    .render_colliders = false,
    .render_paths = true,
    .player_collision = true,

    // layers
    .render_ground = true,
    .render_objects = true,
    .render_tips = true,
};

config_t* get_config_ptr(void) {
    return &global_config;
}
