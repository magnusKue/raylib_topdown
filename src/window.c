#include <stdio.h>
#include <raylib.h>

#include "../include/camera.h"
#include "../include/player.h"
#include "../include/window.h"

void init_window(window_t* window, char* title, int width, int height, float scaler) {
    InitWindow(width, height, title);
    window->rtex = LoadRenderTexture(
        (int)width*scaler, 
        (int)height*scaler
    );
    window->scaler=scaler;

    // SetTargetFPS(120);
}

void start_render(window_t* window, camera_t* camera) {
    BeginTextureMode(window->rtex);
    ClearBackground(BLACK);
    BeginMode2D(camera->cam);
}

void render_to_window(window_t* window, camera_t* camera) {
    EndMode2D();
    EndTextureMode();
    
    // render rtex to screen
    BeginDrawing();
        ClearBackground(RED);     // Clear screen background

        // Draw render texture to screen, properly scaled
        DrawTexturePro(
            window->rtex.texture, 
            (Rectangle){ 
                0.0f, 0.0f, 
                (float)window->rtex.texture.width, (float)-window->rtex.texture.height 
            }, 
            (Rectangle){ 
                0.0f, 0.0f,       
                (float) GetScreenWidth(), (float)GetScreenHeight(), 
            }, 
            (Vector2) { 0.0f, 0.0f }, 
            0.0f, 
            WHITE
        );

        // Draw FPS
        char fps[12];
        sprintf(fps, "%11d", GetFPS());

        DrawText(fps, 5.0, 5.0, 16.0, WHITE);
    EndDrawing();
}
