#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h>  // Controller chip library
#include "TouchScreen.h"    // Library for TouchScreen
#include "./block.h"
#include "./game.h"
#include "./data_handler.h"   // Contains determine_block() function
#include "./draw_handler.h"

// Draws outcome (0 is lose, 1 is win, 2 is tie)
void draw_outcome(Adafruit_ILI9341 display, int win_status) {
  // Need to do this to rotate text
  display.setRotation(1);

  // Set text size and color
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(10);

  // Have to make cursor x different for each case of character sizes
  if (win_status == 0) {
    display.setCursor(45, 85);
    display.fillScreen(ILI9341_RED);
    display.print("Lose");

  } else if (win_status == 1) {
    display.setCursor(50, 85);
    display.fillScreen(ILI9341_GREEN);
    display.print("Win!");

  } else if (win_status == 2) {
    // Use black text because white is hard to see on yellow background
    display.setTextColor(ILI9341_BLACK);
    display.setCursor(50, 85);
    display.fillScreen(ILI9341_YELLOW);
    display.print("Tie!");
  }
}

// Draws a main menu
void draw_menu(Adafruit_ILI9341 display) {
  // Need to do this to rotate text
  display.setRotation(1);

  // Fill screen with blue
  display.fillScreen(ILI9341_BLUE);

  // Set text size and color
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(10);

  display.setCursor(20, 85);
  display.print("PLAY!");
}

// Draw menu for # of players selection
// Unused, will be useful if we decide to implement an AI later
void draw_menu_mode(Adafruit_ILI9341 display) {
  int char_size = 10;
  // Need to do this to rotate text
  display.setRotation(1);

  display.fillRect((display.width() / 2), 0, (display.width() / 2),
                    display.height(), ILI9341_BLUE);
  display.fillRect(0, 0, (display.width() / 2), display.height(), ILI9341_PINK);

  // Characters are (5,7) * char_size
  // Using this expression, we can center characters
  display.drawChar(((display.width() / 4) - (char_size * (5 / 2))),
                  ((display.height() / 2) - (char_size * (7 / 2))),
                  '1', ILI9341_WHITE, ILI9341_PINK, char_size);
  display.drawChar((((3 * display.width()) / 4) - (char_size * (5 / 2))),
                  ((display.height() / 2)- (char_size * (7 / 2))),
                  '2', ILI9341_WHITE, ILI9341_BLUE, char_size);
}

// Draws a waiting screen when waiting for a response from the other arduino
void draw_waiting(Adafruit_ILI9341 display) {
  // Need to do this to rotate text
  display.setRotation(1);

  // Fill screen with black
  display.fillScreen(ILI9341_BLACK);

  // Set text size and color
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(5);

  display.setCursor(40, 103);
  display.print("Waiting.");
}

// Draws an empty grid
void draw_empty_grid(Adafruit_ILI9341 display, int BOXSIZE) {
  // Need to do this to rotate to proper game orientation
  display.setRotation(0);

  display.fillScreen(ILI9341_BLACK);

  // Draws a 6 x 7 grid of 40x40 pixel boxes
  // Starts at index 0 because 40*0=0
  for (int i = 1; i < 8; i++) {
    for (int j = 1; j < 7; j++) {
      display.drawRect((display.width() - BOXSIZE * j),
      (display.height() - BOXSIZE * i), BOXSIZE, BOXSIZE, ILI9341_WHITE);
    }
  }
}

// Draws a grey confirm button
void draw_grey_confirm(Adafruit_ILI9341 display, int BOXSIZE) {
  display.fillRect(120, 0, 120, BOXSIZE, 0x8410);
}

// Draws a green confirm button
void draw_green_confirm(Adafruit_ILI9341 display, int BOXSIZE) {
  display.fillRect(120, 0, 120, BOXSIZE, ILI9341_GREEN);
}

// Draws a red cancel button
void draw_cancel(Adafruit_ILI9341 display, int BOXSIZE) {
  display.fillRect(0, 0, 120, BOXSIZE, ILI9341_RED);

  /*  // displays "RESET" on the red block. Un-comment if you want
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(2);
  display.setCursor(30, 13);
  display.print("RESET");
  */
}

// Draws an empty battleship map
void draw_empty_map(Adafruit_ILI9341 display, int BOXSIZE) {
  draw_empty_grid(display, BOXSIZE);
  draw_grey_confirm(display, BOXSIZE);
  draw_cancel(display, BOXSIZE);
}

// Fill in grid box at a given location
void draw_at_grid_pos(Adafruit_ILI9341 display, int BOXSIZE,
                      String grid_pos, int color) {
  char let[7] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
  char num[6] = {'0', '1', '2', '3', '4', '5'};
  // Indice shifted because of the menu on the left side of the screen.
  for (int i = 7; i > 0; i--) {
    for (int j = 5; j > -1; j--) {
      if (let[i - 1] == grid_pos[0] && num[j] == grid_pos[1]) {
        // Fill the rectangle
        display.fillRect((BOXSIZE * j), (BOXSIZE * i), BOXSIZE,
                         BOXSIZE, color);
        // Redraw the white border
        display.drawRect((BOXSIZE * j), (BOXSIZE * i), BOXSIZE,
                         BOXSIZE, ILI9341_WHITE);
      }
    }
  }
}

// Clears selected tile(s)
void clear_all_selections(Adafruit_ILI9341 display, int BOXSIZE,
                          String block_array[], int length) {
  for(int i = 0; i < length; i++) {
    // Draw black at every previously selected square
    draw_at_grid_pos(display, BOXSIZE, block_array[i], ILI9341_BLACK);
  }
  draw_grey_confirm(display, BOXSIZE);  // Draws a grey confirm button
}

/*
0 = undisturbed
1 = has been shot but no boat
2 = has a boat hidden; not shot
3 = has a boat that's been shot
4 = has a full boat sunk
5 = enemy's: shot but no boat
6 = enemy's: shot and there is a boat
7 = enemy's: full boat sunk
8 = enemy's: has boat but not shot
*/
// Draws state-relevent color at given grid position
void draw_state(Adafruit_ILI9341 display, int BOXSIZE,
                String grid_pos, int state) {
  switch(state){
    case 0:
    case 8:
       // Case 0 is the same as case 8 for the purpose of drawing
       // Draws black tile if nothing has happened
      draw_at_grid_pos(display, BOXSIZE, grid_pos, ILI9341_BLACK);
      break;
    case 1:
    case 5:
      // Shot but no boat: BLUE
      draw_at_grid_pos(display, BOXSIZE, grid_pos, ILI9341_BLUE);
      break;
    case 2:
      // Your own boat hidden: GREY
      draw_at_grid_pos(display, BOXSIZE, grid_pos, 0x8410);
      break;
    case 3:
    case 6:
      // Boat hit but not sunk: ORANGE
      draw_at_grid_pos(display, BOXSIZE, grid_pos, 0xFD20);
      break;
    case 4:
    case 7:
      // All parts of the boat dead: RED
      draw_at_grid_pos(display, BOXSIZE, grid_pos, ILI9341_RED);
      break;
  }
}

// Blinks a selected grid position a given amount of times
// mode 0: show my enemy's shot on my board
// mode 1: show my shot on my enemy's board
void blink_block(Adafruit_ILI9341 display, int BOXSIZE, Block play_arr[], String *grid_pos, int blink_times, int mode) {
  int block_number = determine_array_element(*grid_pos);  // Converts grid position to grid number
   int previous_state = determine_previous_state(play_arr, block_number);  // Determine the state before it was shot

  for(int i = 0; i < blink_times; i++) {
    if(mode == 0){
      delay(500); // Half second delay between transitions
      draw_state(display, BOXSIZE, *grid_pos, previous_state);
      delay(500);
      draw_state(display, BOXSIZE, *grid_pos, play_arr[block_number].getBlock());

    } else if(mode == 1) {
      delay(500); // Half second delay between transitions
      draw_at_grid_pos(display, BOXSIZE, *grid_pos, ILI9341_BLACK);
      delay(500);
      draw_state(display, BOXSIZE, *grid_pos, play_arr[block_number].getEnemy());
    }
  }
}

// Draws the message "opponent" vertically on the left side of the screen
void draw_opponent_message(Adafruit_ILI9341 display) {
  // Set text size and color
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(2);

  display.setCursor(60, 13);
  display.print("YOUR BOATS");
}

// Draws appropriate color at every grid position on our own board
void draw_board_self(Adafruit_ILI9341 display, int BOXSIZE, Block play_arr[], String *opponent_selection) {
  // Need to do this to rotate to standard game rotations as specified in readme (after main menu).
  display.setRotation(0);
  draw_empty_grid(display, BOXSIZE);
  for(int i = 0; i < 42; i++){
    // Draw the state for each self block
    draw_state(display, BOXSIZE, determine_block(i), play_arr[i].getBlock());
  }
  // Draw the word "opponent" to know that it's our opponent's shot
  draw_opponent_message(display);
  // Blink the last the opponent shot block 2 times
  blink_block(display, BOXSIZE, play_arr, opponent_selection, 2, 0);
}

// Draws appropriate color at every grid position on the enemy's board
void draw_board_enemy(Adafruit_ILI9341 display, int BOXSIZE, Block play_arr[], String *my_selection) {
  draw_empty_map(display, BOXSIZE);
  for(int i = 0; i < 42; i++) {
    // Draw the state for each self block
    draw_state(display, BOXSIZE, determine_block(i), play_arr[i].getEnemy());
  }
  // Blink the last block I shot 2 times
  blink_block(display, BOXSIZE, play_arr, my_selection, 2, 1);
}

/* 
 * This function draws the grey block that
 * tells the user how many blocks to press
 * SELECT + <a number>
 */
void draw_select(Adafruit_ILI9341 display, int BOXSIZE, String message){
  display.fillRect(120, 0, 120, BOXSIZE, 0x8410);
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(2);
  display.setCursor(132, 13);
  display.print("SELECT " + message);
}

/* 
 * This function determines how many blocks the user should enter
 * and calls a function to let prompt the user for the boat
 */
void draw_grey_setup(Adafruit_ILI9341 display, int BOXSIZE, int block_number){
  String message = "";

  // if the first boat has not been entered fully
  if ((0<=block_number)&&(block_number<=4)) {message = "5";}

  // if the second boat has not been entered fully
  else if ((5<=block_number)&&(block_number<=8)) {message = "4";}

  // if the thrid boat has not been entered fully
  else if ((9<=block_number)&&(block_number<=13)) {message = "3";}

  // This should never execute
  else {Serial.println("Error in draw_grey_setup.");}

  // Calls the function that actually does the drawing
  draw_select(display, BOXSIZE, message);
}
