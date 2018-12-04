#ifndef _BOAT_ID_H_
#define _BOAT_ID_H_

int determine_first_block_type(String grid_pos);

int valid_second_block(String grid_pos1, String grid_pos2);

int special_case(int block_numbers[], int cardinal);

int valid_consequent_blocks(String grid_pos[], int squares_selected, int cardinal);

int first_contact(String block_arr[], int squares_selected, int squares_allowed);

void input_boat_id(String selected_array[], Block game_arr[]);

void input_enemy_boat_id(String selected_array[], Block game_arr[]);

void freeze_boat(String selected_array[], String frozen_boats[],  int squares_selected);

bool check_not_frozen(String frozen_boats[], String pos, int squares_allowed);

#endif
