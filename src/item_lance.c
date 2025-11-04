# include <assert.h>

# include "../include/item_lance.h"
# include "../include/item.h"
# include "../include/anim.h"
# include "../include/item_meele.h"

item_t* create_item_lance() {
    item_t* item_p = create_meele_item_base();
    item_p->rot_origin_offs = (Vector2) { 0, 0 };

    itemdata_meele_t* data = get_meele_data(item_p->item_data);
    data->animation[0] = (item_transl_t) {(Vector2) {0,0}, (float) 0.0f};
    data->animation[1] = (item_transl_t) {(Vector2) {0,-2}, (float) -40.0f};
    data->animation[2] = (item_transl_t) {(Vector2) {0,-1}, (float) 30.0f};
    data->animation[3] = (item_transl_t) {(Vector2) {0,0}, (float) 20.0f};
    data->animation[4] = (item_transl_t) {(Vector2) {0,0}, (float) 0.0f};

    data->used_frames = 5; // not as index
    data->animation_length_ms = 1000;

    item_p->cooldown_ms = 700;

    item_p->sprite = LoadTexture("assets/sprites/items/lance.png");
    item_p->render_offset = (Vector2) { -8, -5 };

    item_p->type = MEELE_WEAPON;
    item_p->item_action = &meele_item_attack;
    item_p->get_item_anim_transl = &get_item_animation_frame;
    return item_p;
}
