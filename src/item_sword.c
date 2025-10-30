# include <stdlib.h>
# include <assert.h>

# include "../include/item_sword.h"
# include "../include/item.h"
# include "../include/item_meele.h"

item_t* create_item_sword() {

    item_t* item_p = create_meele_item_base();

    // item_p->icon = LoadTexture("");
    item_p->sprite = LoadTexture("assets/sprites/items/sword.png");
    item_p->render_offset = (Vector2) { 4, -5 };

    item_p->type = MEELE_WEAPON;
    item_p->item_action = NULL;
    return item_p;
}
