#ifndef _AI_H_
#define _AI_H_

int choose_direction(String root);

// Choose a new direction based on the old one
int choose_new_direction(String root, int direction);

String *ai_pick_boats();

String *ai_take_shot(Block game_arr[], String AI_last_shot, String root, int direction);

#endif
