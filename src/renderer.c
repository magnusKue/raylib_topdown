#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

# include "../include/renderer.h"

# define DEFAULT_RENDR_ALLOC 1024

renderer_t global_renderer = {
    .buffer = NULL,
    .buffer_counter = 0,
    .buffer_capacity = 0,
};


renderer_t* get_renderer_ptr() {
    return &global_renderer;
}

void init_render_buffer() {
    renderer_t* r = get_renderer_ptr();
    r->buffer = calloc(DEFAULT_RENDR_ALLOC, sizeof(renderdata_t));
    assert(r->buffer);
    r->buffer_capacity = DEFAULT_RENDR_ALLOC;
    r->buffer_counter = 0;
}

void expand_render_buffer() {
    renderer_t* r = get_renderer_ptr();
    // double buffer capacity
    r->buffer = realloc(r->buffer, 2 * sizeof(renderdata_t) * r->buffer_capacity);
    assert(r->buffer);
    r->buffer_capacity *= 2;
}

void push_to_render_buffer(renderdata_t data) {
    renderer_t* r = get_renderer_ptr();
    if (r->buffer_counter >= r->buffer_capacity) {
        expand_render_buffer();
    }
    r->buffer[r->buffer_counter++] = data;
}

void free_render_buffer() {
    renderer_t* r = get_renderer_ptr();
    free(r->buffer);
    r->buffer = NULL;
}

void clear_render_buffer() {
    renderer_t* r = get_renderer_ptr();
    r->buffer_counter = 0;
}

int compare_objects(const void *a, const void *b) {
    const renderdata_t *objA = (const renderdata_t *)a;
    const renderdata_t *objB = (const renderdata_t *)b;

    if (objA->ypos < objB->ypos) return -1;
    if (objA->ypos > objB->ypos) return 1;
    return 0;
}

void sort_render_buffer() {
    renderer_t* r = get_renderer_ptr();
    qsort(r->buffer, r->buffer_counter, sizeof(renderdata_t), compare_objects);
}

void ysort_and_render_to_screen() {
    renderer_t* r = get_renderer_ptr();
    
    sort_render_buffer();

    for (int i = 0; i < r->buffer_capacity; i++) {
        // render
        int type = r->buffer[i].type;
        renderdata_t* data = &r->buffer[i];
        switch (type) {
            case TEXTURE_PRO:
                DrawTexturePro(data->texture, data->src, data->dest, data->origin, data->rotation, data->tint);
                break;
                // TODO: implement
            default:
                printf("[!!] Unimplemented renderer! Index: %d", type);
        }
    }
}

