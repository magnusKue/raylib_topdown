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
    // Draw FPS
    char fps[12];
    sprintf(fps, "%11d", GetFPS());

    DrawText(fps, 5.0, 5.0, 16.0, WHITE);

    EndMode2D();
    EndDrawing();
}
