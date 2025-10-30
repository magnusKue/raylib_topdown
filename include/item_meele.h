typedef struct ItemDataMeele {
    int damage;

} itemdata_meele_t;

itemdata_meele_t* get_meele_data(void* data) {
    return (itemdata_meele_t*) data;
}

