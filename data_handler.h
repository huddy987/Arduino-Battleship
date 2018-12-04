#ifndef _DATA_HANDLER_H_
#define _DATA_HANDLER_H_

uint8_t determine_array_element(String grid_pos);

String determine_block(uint8_t block_number);


/*
  The following four function shows the game on serial-mon
  These are for debugging purposes
*/

// For printing user's block states
void print_blocks(Block player_array[]);

// print enemy's block states from user's POV
void print_blocks_2(Block player_array[]);

// print user's boat id
void print_blocks_3(Block player_array[]);

// print enemy's boat id
void print_blocks_4(Block player_array[]);


/*
  The following three function deals with user's blocks
*/

// checks if a specific boat has been sunk
bool check_my_boat_sunk(Block play_arr[], uint8_t boat_id);

// if a boat has been sunk, converts it to state 4
void kill_my_boat(Block play_arr[], uint8_t boat_id);

// checks if all of the boats have been sunk
void check_if_my_boat_sunk(Block play_arr[]);



// determines the new state of the block that the enemy shot
uint8_t recieve_turn(Block play_arr[], uint8_t boat_block_number);


/*
  The following three function deals with enemy's blocks
*/

// checks if a specific boat has been sunk
bool check_enemy_boat_sunk(Block play_arr[], uint8_t boat_id);
// if a boat has been sunk, converts it to state 4
void kill_enemy_boat(Block play_arr[], uint8_t boat_id);
// checks if all of the boats have been sunk
void check_if_enemy_boat_sunk(Block play_arr[]);


// determines the new block state of the block that the user shot
uint8_t update_my_array(Block play_arr[], uint8_t my_block_number);

// checks if the user is dead
bool check_self_death(Block play_arr[], uint8_t blocks_allowed);

// checks if the enemy is dead
bool check_enemy_death(Block play_arr[], uint8_t blocks_allowed);

// Check if you have lost or your enemy has lost, and set gamestate to 3 if it is
bool check_deaths(Block play_arr[], int squares_allowed, Game *game);

// Determines block state before it was shot
int determine_previous_state(Block play_arr[], uint8_t my_block_number);

#endif
