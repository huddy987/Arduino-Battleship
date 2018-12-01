#ifndef _DATA_HANDLER_H_
#define _DATA_HANDLER_H_

uint8_t determine_array_element(String grid_pos);

String determine_block(uint8_t block_number);

uint8_t determine_first_block_type(String grid_pos);

int valid_second_block(String grid_pos1, String grid_pos2);

bool valid_third_block(String grid_pos1, String grid_pos2, String grid_pos3, uint8_t block_type);

void print_blocks(Block player_array[]);

void print_blocks_2(Block player_array[]);


bool check_all_boat_sunk(Block play_arr[], uint8_t boat_id);

uint8_t kill_entire_boat(Block play_arr[], uint8_t boat_id);

void send_boat_death(Block play_arr[], uint8_t boat_id, uint8_t boat_death);

uint8_t recieve_turn(Block play_arr[], uint8_t boat_block_number);

uint8_t update_my_array(Block play_arr[], uint8_t my_block_number);

void making_a_turn(String grid_pos, Block player_array[]);

bool check_self_death(Block play_arr[], uint8_t blocks_allowed);

bool check_enemy_death(Block play_arr[], uint8_t blocks_allowed);

// Check if you have lost or your enemy has lost, and set gamestate to 3 if it is
bool check_deaths(Block play_arr[], int squares_allowed, Game *game);

// Determines block state before it was shot
int determine_previous_state(Block play_arr[], uint8_t my_block_number);

#endif
