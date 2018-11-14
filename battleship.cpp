#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen
#include "touch_handler.h"  // touch handler header file
#include "draw_handler.h" // draw handler header file


// These are the four touchscreen analog pins
#define YP A10  // must be an analog pin, use "An" notation!
#define XM A11  // must be an analog pin, use "An" notation!
#define YM 6   // can be any digital pin
#define XP 7   // can be any digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// Calibrate minimum pressure to be considered a touch
#define MINPRESSURE 10

// The display uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9

// Define Display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Define TouchScreen
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Define grid box size (40x40)
#define BOXSIZE 40

void setup() {
  init();
  Serial.begin(9600);
  Serial.println("Welcome to Battleship!");

  tft.begin();
  // draw menu
  draw_menu(tft);
}

int main(){
  setup();
  while (1){
    loop();
  }
  //TODO: while setupphase();
  //TODO: while gamephase();
  //TODO: While endphase();

  return 0;
}

void loop(){
  // Get a point
  TSPoint p = ts.getPoint();

  // If the point has not enough pressure, repeat the loop (no press registers)
  if (p.z < MINPRESSURE) {
   return;
  }

  // Scale from 0->1000 to tft.width and tft.height using the calibration #'s

  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width() + 10);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

  get_game_mode(tft, p);

  /*
  Serial.println(p.x);
  Serial.println(p.y);
  // If we are inside the menu region...
  if (p.y < 40){
    // If we are in the top half of the menu...
    if (p.x > 120){
      // Draw menu
      draw_menu(tft);
    }
    // If we are in the bottom half of the menu...
    else if (p.x < 120) {
      tft.fillRect(0, 0, 120, BOXSIZE, ILI9341_GREEN);
      delay(500);
      tft.fillRect(0, 0, 120, BOXSIZE, ILI9341_RED);
    }
  }

  // If we are inside the grid region get the grid position
  else if (p.y > 40){
    String grid_pos = get_grid_position(p, BOXSIZE);
    draw_at_grid_pos(tft, BOXSIZE, grid_pos, ILI9341_NAVY);
    Serial.print(grid_pos);
    delay(500);

      Since this is a string, it will be very easy for decomposition:
      grid_pos[0] returns the A-G value, grid_pos[1] returns the 0-5 value
    */
}
