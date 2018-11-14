// FILE FOR TOUCH SENSING RELATED CODE

#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen
#include "touch_handler.h"  //touch handler header file
#include "draw_handler.h" // Draw handler header file

// Check how many players are selected and go to appropriate game mode
void get_game_mode(Adafruit_ILI9341 display, TSPoint point){
  if (point.x > 120){
    //TODO: Single player
    Serial.print("1 player mode selected.");
    draw_empty_grid(display, 40);
  }
  else if (point.x < 120){
    //TODO: 2 players
    Serial.print("2 players mode selected.");
    draw_empty_grid(display, 40);
  }
}

// Returns the grid position in the for [LETTER][NUMBER] as a string

String get_grid_position(TSPoint point, int BOXSIZE){
  // Grid box constants
  char let[7] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
  char num[6] = {'0', '1', '2', '3', '4', '5'};
  String grid_pos;
  // Start at the largest box size
  // NOTE: It may seem strange, but this one is shifted up an index since the menu is on the side
  for (int i = 7; i > 0; i--){
    // If the y coordinate is greater then the multiple of the box, we know it is inside the box at that index.
    if (point.y >= (BOXSIZE * i)){
      // Start at the largest box size
      for (int j = 5; j > -1; j--){
        // If the x coordinate is greater then the multiple of the box, we know it is inside the box at that index.
        if (point.x >= (BOXSIZE * j)){
          // Set the value of the grid position
          grid_pos = String(let[(i - 1)]) + String(num[j]);
          // Break here so we don't continually draw boxes in the x direction
          return grid_pos;
        }
      }
      // Break here so we don't continually draw boxes in the y direction
      return grid_pos;
    }
  }
  /*
    We should never hit this part of the function if we are using it properly;
    this should only be called in the grid region.
  */
  return "Error";
}
