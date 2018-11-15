#ifndef _DRAW_HANDLER_H_
#define _DRAW_HANDLER_H_


void draw_outcome(Adafruit_ILI9341 display, int win_status);

void draw_menu(Adafruit_ILI9341);

void draw_grey_confirm(Adafruit_ILI9341 display, int BOXSIZE);

void draw_green_confirm(Adafruit_ILI9341 display, int BOXSIZE);

void draw_cancel(Adafruit_ILI9341 display, int BOXSIZE);

void draw_empty_grid(Adafruit_ILI9341 display, int BOXSIZE);

void draw_at_grid_pos(Adafruit_ILI9341 display, int BOXSIZE, String grid_pos, int color);


#endif
