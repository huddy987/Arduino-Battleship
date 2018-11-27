#ifndef _DRAW_HANDLER_H_
#define _DRAW_HANDLER_H_


// Draws outcome (0 is lose, 1 is win, 2 is tie)
void draw_outcome(Adafruit_ILI9341 display, int win_status);


// Draws the original menu to select 1 or 2 players
void draw_menu(Adafruit_ILI9341);

// Draws a waiting screen when waiting for other arduino response
void draw_waiting(Adafruit_ILI9341);

// Draws a greyed out confirm button
void draw_grey_confirm(Adafruit_ILI9341 display, int BOXSIZE);

// Draws a green confirm button
void draw_green_confirm(Adafruit_ILI9341 display, int BOXSIZE);

// Draws a cancel button
void draw_cancel(Adafruit_ILI9341 display, int BOXSIZE);

// Draws a grid with all black boxes
void draw_empty_grid(Adafruit_ILI9341 display, int BOXSIZE);

// Draws an empty battleship map
void draw_empty_map(Adafruit_ILI9341 display, int BOXSIZE);

// Draws a color at a given grid position
void draw_at_grid_pos(Adafruit_ILI9341 display, int BOXSIZE, String grid_pos, int color);

// Clears selected tile(s)
void clear_all_selections(Adafruit_ILI9341 display, int BOXSIZE, String block_array[], int length);

// Draws state-relevent color at given grid position
void draw_state(Adafruit_ILI9341 display, int BOXSIZE, String grid_pos, int state);


#endif
