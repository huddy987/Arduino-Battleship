#ifndef _TOUCH_HANDLER_H_
#define _TOUCH_HANDLER_H_

// header file for touch handler

// Gets a point and maps it to the screen dimensions
TSPoint get_point(Adafruit_ILI9341 tft, TouchScreen ts);

// Checks if confirm or cancel button was pressed
int get_confirm_or_cancel(TSPoint point);


// Gets the box selected and returns it in the from "[letter][number]"
String get_grid_position(TSPoint point, int BOXSIZE);

// Loops continually until a press is registered
void wait_for_touch(Adafruit_ILI9341 tft, TouchScreen ts, int MINPRESSURE);

#endif
