#include <stdio.h>
#include <raylib.h>

#include "../include/camera.h"
#include "../include/player.h"
#include "../include/window.h"

void init_window(char* title, int width, int height, int maxFps) {
    InitWindow(width, height, title);
    SetTargetFPS(600);
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
    char fps[12];
    sprintf(fps, "%d", GetFPS());
    DrawText(fps, 10.0, 10.0, /*FNT SIZE*/16.0, WHITE);

    EndDrawing();
}
