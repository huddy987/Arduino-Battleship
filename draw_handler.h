#ifndef _DRAW_HANDLER_H_
#define _DRAW_HANDLER_H_

// header file for touch handler

void draw_empty_grid(Adafruit_ILI9341 display, int BOXSIZE);

void draw_at_grid_pos(Adafruit_ILI9341 display, int BOXSIZE, String grid_pos, int color);


#endif
