#include "../include/config.h"

// Initialize debug_mode to false or true as needed
bool debug_mode = false;

void set_debug_mode(bool value) {
    debug_mode = value;
}

bool get_debug_mode(void) {
    return debug_mode;
}

bool toggle_debug_mode(void) {
    debug_mode = !debug_mode;
}
