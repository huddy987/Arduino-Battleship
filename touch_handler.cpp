// FILE FOR TOUCH SENSING RELATED CODE

#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h>  // Controller chip library
#include "TouchScreen.h"    // Library for TouchScreen
#include "./touch_handler.h"  // Touch handler header file

// Get a point, and map it to the screen dimensions
TSPoint get_point(Adafruit_ILI9341 tft, TouchScreen ts) {
  // This is calibration data for the raw touch data to the screen coordinates
  // Thanks to https://github.com/adafruit/ for the library
  #define TS_MINX 150
  #define TS_MINY 120
  #define TS_MAXX 920
  #define TS_MAXY 940

  // Get a point
  TSPoint p = ts.getPoint();

  // Scale from 0->1000 to tft.width and tft.height using the calibration #'s
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width() + 10);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

  return p;
}


// Check if confirm or cancel has been pressed
// Returns: 0 (none), 1 (confirm), 2 (cancel)
int get_confirm_or_cancel(TSPoint point) {
  // If we are in the menu region
  if (point.y < 25) {
    // If we are in the confirm region, return 1
    if (point.x > 120) {
      return 1;
    // If we are in the cancel region, return 2
    } else if (point.x < 120) {
      return 2;
    }
  }
  // If we are not in the menu region, return 0
  return 0;
}

// Returns the grid position in the [LETTER][NUMBER] notation as a string
String get_grid_position(TSPoint point, int BOXSIZE) {
  // Grid box constants
  char let[7] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
  char num[6] = {'0', '1', '2', '3', '4', '5'};
  String grid_pos;
  // Start at the largest box size
  // This one is shifted up an index since the menu is on the side
  for (int i = 7; i > 0; i--) {
    // If the y coordinate is greater then the multiple of the box,
    // we know it is inside the box at that index.
    if (point.y > (BOXSIZE * i)) {
      // Start at the largest box size
      for (int j = 5; j > -1; j--) {
        // If the x coordinate is greater then the multiple of the box,
        // we know it is inside the box at that index.
        if (point.x > (BOXSIZE * j)) {
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
  return NULL;
}

// Loops continually until a press is registered
void wait_for_touch(Adafruit_ILI9341 tft, TouchScreen ts, int MINPRESSURE) {
  while (true) {
    TSPoint p = get_point(tft, ts);
    if (p.z > MINPRESSURE) {
      break;
    }
  }
}
