#include <stdio.h>
#include <raylib.h>

#include "../include/camera.h"
#include "../include/player.h"
#include "../include/window.h"
#include "../include/config.h"

void init_window(char* title, int width, int height, int maxFps) {
    InitWindow(width, height, title);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(1000);
}

void start_render(camera_t* camera) {
    // BeginTextureMode(window->rtex);
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera->cam);
}

void render_to_window(camera_t* camera) {
    // End camera mode
    EndMode2D();

    // Draw FPS
    
    if (get_config_ptr()->render_colliders) {
        char fps[12];
        sprintf(fps, "%d", GetFPS());
        DrawText(fps, 10.0, 10.0, /*FNT SIZE*/24.0, WHITE);
    }
    EndDrawing();
}
