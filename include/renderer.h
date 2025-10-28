# include <raylib.h>

typedef enum Rendertype {
    TEXTURE_PRO,
} rendertype_t;

typedef struct Renderdata {
    // texture pro
    rendertype_t type;
    float ypos;
    Texture texture;
    Rectangle src;
    Rectangle dest;
    Vector2 origin;
    float rotation;
    Color tint;
} renderdata_t;

typedef struct Renderer {
   renderdata_t* buffer; 
   int buffer_counter;
   int buffer_capacity;
} renderer_t;


extern renderer_t global_renderer;

renderer_t* get_renderer_ptr();

void init_render_buffer();

void clear_render_buffer();

void push_to_render_buffer(renderdata_t data);

void free_render_buffer();

void ysort_and_render_to_screen();
