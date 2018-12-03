#ifndef _BOAT_ID_H_
#define _BOAT_ID_H_

int determine_first_block_type(String grid_pos);

int valid_second_block(String grid_pos1, String grid_pos2);

int valid_third_block(String grid_pos[], int squares_selected);
int valid_fourth_block(String grid_pos[], int squares_selected);
int valid_fifth_block(String grid_pos[], int squares_selected);

int first_contact(String block_arr[], int squares_selected, int squares_allowed);

void input_boat_id(String selected_array[], Block game_arr[]);

void input_enemy_boat_id(String selected_array[], Block game_arr[]);

#endif
