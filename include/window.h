#include <raylib.h>
#include "../include/camera.h"

typedef struct Window {
    RenderTexture2D rtex;
    float scaler;
} window_t;

void init_window(window_t* window, char* title, int width, int height, float scaler);
void start_render(window_t* window, camera_t* camera);
void render_to_window(window_t* window, camera_t* camera);
