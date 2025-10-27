/* Samay Patel */

bool add_a_rock(struct Sim * sim_ptr, struct Rock * tempRock);
void all_rocks_bump_check(struct Sim * sim_ptr);
void draw_a_rock(struct Rock *rock);
void move_a_rock(struct Rock * rock);
void print_a_rock(struct Rock * rock);
void rock_cleanup(struct Sim * sim_ptr);
void rock_iterator (struct Sim * sim_ptr, void (*callback) (struct Rock *));
void rocks_shots_hit_message( struct Shot *shot, struct Rock *rock);
void shot_to_rock_bumps_check(struct Sim * sim_ptr);
