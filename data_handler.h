#ifndef _DATA_HANDLER_H_
#define _DATA_HANDLER_H_

uint8_t determine_array_element(String grid_pos);

String determine_block(uint8_t block_number);

/*
  Inputs: String block - A0 to G5
  Output:  uint8_t number corresponding to where the block is:
    1. Top row
    2. Bottom row
    3. Left column
    4. Right column
    5. Centre tile

    Corner tiles will return negative values
    they have two constraints and will be especially dealt with
    -1 : top left (35)
    -2 : top right (41)
    -3 : bottom left (0)
    -4 : bottom right (6)
*/
uint8_t determine_first_block_type(String grid_pos);

/*
  Important: Its the second block minus the first one
  returns true if the block selected is allowed

  Inputs: String grid_pos1 - the first block selected
          String grid_pos2 - the second block selected
          block_type - result of determine_first_block_type(1st block selected)
  Output: 0 - if the block is invalid
          if valid, returns (block2_number - block1_number)
*/
uint8_t valid_second_block(String grid_pos1, String grid_pos2, uint8_t first_block_type);

/*
  Important: Its the second block minus the first one
  returns true if the block selected is allowed

  Inputs: String grids[] - maybe the array of
          String grid_pos - the block selected
          block_type - result of valid_second_block
  Output: false - invalid block
          true  - valid block
*/
bool valid_third_block(String grid_pos1, String grid_pos2, String grid_pos3, uint8_t block_type);
//

// print my states
void print_blocks(Block player_array[]);

// print enemy states
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


#endif
