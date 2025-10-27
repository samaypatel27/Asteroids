/* Samay Patel */

bool add_a_ship(struct Sim * sim_ptr, struct Ship * ship);
bool always_true(void * data, void * helper) ;
bool compare_xPos(void * data1, void * data2);
void create_shot(struct Ship * ship);
void draw_a_ship(struct Ship * ship);
void draw_a_shot(void * data);
void fire_message( struct Ship *ship);
void leaves_message( struct Shot *shot);
void move_a_ship(struct Ship * ship);
void move_a_shot(void * data);
bool out_of_bounds(void * data, void * helper);
void print_a_ship(struct Ship * ship);
void print_a_shot(void * data);
void ship_iterator(struct Sim * sim_ptr, void (*callback) (struct Ship *));
void ship_to_rock_collision(struct Ship * ship);
