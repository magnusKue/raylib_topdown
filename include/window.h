# pragma once

#include <raylib.h>
#include "../include/camera.h"

void init_window(char* title, int width, int height, int maxFps);
void start_render(camera_t* camera);
void render_to_window(camera_t* camera);
