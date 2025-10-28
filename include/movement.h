typedef struct Enemy enemy_t;
typedef struct Entity entity_t;
typedef struct Player player_t;
typedef struct World world_t;


void entity_move_by_velocity(entity_t* entity);
void entity_apply_friction(entity_t* entity);

int entity_move_and_collide(entity_t* entity, world_t* world);
int enemy_follow_path(enemy_t* enemy, player_t* player, world_t* world);
