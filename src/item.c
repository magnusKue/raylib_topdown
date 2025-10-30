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

