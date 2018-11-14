#ifndef _TOUCH_HANDLER_H_
#define _TOUCH_HANDLER_H_

// header file for touch handler

void get_game_mode(Adafruit_ILI9341 display, TSPoint point);

String get_grid_position(TSPoint point, int BOXSIZE);


#endif
