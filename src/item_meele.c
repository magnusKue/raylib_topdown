# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "../include/item_meele.h"
# include "../include/item.h"

itemdata_meele_t* get_meele_data(void* data) {
    return (itemdata_meele_t*) data;
}

item_t* create_meele_item_base() {
    item_t* item_p = malloc(sizeof(item_t));
    assert(item_p);

    item_p->item_data = (itemdata_meele_t*)malloc(sizeof(itemdata_meele_t));
    assert(item_p->item_data);

    return item_p;
}

void meele_item_attack(player_t* player, world_t* world) {
    printf("TODO: make it attack stuff\n");

}

