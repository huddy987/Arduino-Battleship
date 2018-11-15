#ifndef _COMMUNICATION_HANDLER_H_
#define _COMMUNICATION_HANDLER_H_

// just converts the grid_position into the array element to make indexing easier
uint8_t determine_array_element(String grid_pos);

/*
  Input: player array and the boat id
  Output: 1 if all parts of the boat has been shot
          0 otherwise
*/
uint8_t check_all_boat_sunk(uint8_t play_arr[][2], uint8_t boat_id);

// makes all parts of the boat with the same boat id = 4
// meaning the entire boat is dead
void kill_entire_boat(uint8_t play_arr[][2], uint8_t boat_id);

void recieve_turn(uint8_t player_array[][2], uint8_t block_number);

void making_a_turn(String grid_pos,uint8_t player_array[][2]);


#endif
