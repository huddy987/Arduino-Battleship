#ifndef _TOUCH_HANDLER_H_
#define _TOUCH_HANDLER_H_

// header file for touch handler

TSPoint get_point(Adafruit_ILI9341 tft, TouchScreen ts);

int get_game_mode(Adafruit_ILI9341 display, TSPoint point);

int get_confirm_or_cancel(TSPoint point);

String get_grid_position(TSPoint point, int BOXSIZE);



#endif
