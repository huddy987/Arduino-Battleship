#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen


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

//The grid boxes are 40x40 pixels
#define BOXSIZE 40

void setup() {
  init();
  Serial.begin(9600);
  Serial.println("Welcome to Battleship!");

  tft.begin();
  // Fill the screen with black (for background)
  tft.fillScreen(ILI9341_BLACK);

  // Draws a 6 x 7 grid of 40x40 pixel boxes
  for (int i = 1; i < 8; i++){
    for (int j = 1; j < 7; j++){
      tft.drawRect((tft.width() - BOXSIZE * j), (tft.height() - BOXSIZE * i), BOXSIZE, BOXSIZE, ILI9341_WHITE);
    }
  }
  // Draw a placeholder for the menu
  // TODO: Replace this with something meaningful/useful
  tft.fillRect(0, 0, 120, BOXSIZE, ILI9341_GREEN);
  tft.fillRect(120, 0, 120, BOXSIZE, ILI9341_RED);
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

  //WARNING: Oddly enough, there is a deadzone at the top of the display if we don't add the +10. - Hudson
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width() + 10);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

  // TODO: Round these values to the center of each grid box and check if touch is inside a menu button.
  Serial.println(p.x);
  Serial.println(p.y);

  // Placeholder for testing touch screen accuracy
  tft.fillCircle(p.x, p.y, 1, ILI9341_ORANGE);

}
