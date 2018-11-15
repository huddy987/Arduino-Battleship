#ifndef _DATA_HANDLER_H_
#define _DATA_HANDLER_H_

uint8_t determine_array_element(String grid_pos);

uint8_t check_all_boat_sunk(uint8_t play_arr[][2], uint8_t boat_id);

void kill_entire_boat(uint8_t play_arr[][2], uint8_t boat_id);

void recieve_turn(uint8_t player_array[][2], uint8_t enemy_block_number);

void kill_entire_enemy_boat(uint8_t play_arr[][2], uint8_t boat_id);

void update_my_array(uint8_t player_array[][2], uint8_t my_block_number);

void making_a_turn(String grid_pos,uint8_t player_array[][2]);

void print_array(uint8_t play_arr[][2]);

#endif
