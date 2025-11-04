# include <stdlib.h>
# include <stdio.h>

# include "../include/item.h" 
# include "../include/player.h" 

void free_item(item_t* item) {
    free(item->item_data);
    free(item);
}

void do_item_action(player_t* player, world_t* world) {
    item_t* item = player->current_item;

    if (!item) { return; }
    if (!item->item_action) { return; }

    if (get_time_since_ms(player->last_item_action) >= item->cooldown_ms) {
        item->item_action(player, world);
        gettimeofday(&player->last_item_action, NULL);
    }
}

item_transl_t lerp_item_transl(item_transl_t a, item_transl_t b, float factor) {
    item_transl_t new_transl = a;
    new_transl.position.x += factor * (b.position.x-new_transl.position.x);
    new_transl.position.y += factor * (b.position.y-new_transl.position.y);
    new_transl.rotation += factor * (b.rotation-new_transl.rotation);
    return new_transl;
}
