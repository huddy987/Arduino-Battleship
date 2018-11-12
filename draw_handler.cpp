#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen

// Draw menu for player selection

void draw_menu(Adafruit_ILI9341 display){
  int char_size = 10;
  // Need to do this to rotate text
  display.setRotation(1);

  display.fillRect((display.width() / 2), 0, (display.width() / 2), display.height(), ILI9341_BLUE);
  display.fillRect(0, 0, (display.width() / 2), display.height(), ILI9341_PINK);

  // Characters are (5,7) * char_size
  // Using this expression, we can center characters
  display.drawChar(((display.width() / 4) - (char_size * (5 / 2))), ((display.height() / 2) - (char_size * (7 / 2))), '1', ILI9341_WHITE, ILI9341_PINK, char_size);
  display.drawChar((((3 * display.width()) / 4) - (char_size * (5 / 2))), ((display.height() / 2)- (char_size * (7 / 2))), '2', ILI9341_WHITE, ILI9341_BLUE, char_size);
}

// Draws an empty grid
void draw_empty_grid(Adafruit_ILI9341 display, int BOXSIZE){
  display.fillScreen(ILI9341_BLACK);

  // Draws a 6 x 7 grid of 40x40 pixel boxes
  // Starts at index 0 because 40*0=0
  for (int i = 1; i < 8; i++){
    for (int j = 1; j < 7; j++){
      display.drawRect((display.width() - BOXSIZE * j), (display.height() - BOXSIZE * i), BOXSIZE, BOXSIZE, ILI9341_WHITE);
    }
  }
}


// Fill in grid box at a given location
void draw_at_grid_pos(Adafruit_ILI9341 display, int BOXSIZE, String grid_pos, int color){
  char let[7] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
  char num[6] = {'0', '1', '2', '3', '4', '5'};
  // Indice shifted because of the menu on the left side of the screen.
  for (int i = 7; i > 0; i--){
    for (int j = 5; j > -1; j--){
      if (let[i - 1] == grid_pos[0] and num[j] == grid_pos[1]){
        // Fill the rectangle
        display.fillRect((BOXSIZE * j), (BOXSIZE * i), BOXSIZE, BOXSIZE, color);
        // Redraw the white border
        display.drawRect((BOXSIZE * j), (BOXSIZE * i), BOXSIZE, BOXSIZE, ILI9341_WHITE);
      }
    }
  }
}
