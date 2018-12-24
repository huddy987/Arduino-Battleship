#ifndef _AI_H_
#define _AI_H_

int choose_direction(String root);

String *ai_pick_boats();

String *ai_take_shot(Block game_arr[], String AI_last_shot, String root, int direction);

#endif
