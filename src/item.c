# include <stdlib.h>

# include "../include/item.h" 

void free_item(item_t* item) {
    free(item->item_data);
    free(item);
}
