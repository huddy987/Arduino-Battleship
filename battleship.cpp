#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen
#include "touch_handler.h"  // touch handler header file
#include "draw_handler.h" // draw handler header file
#include "game.h"


// These are the four touchscreen analog pins
#define YP A10  // must be an analog pin, use "An" notation!
#define XM A11  // must be an analog pin, use "An" notation!
#define YM 6   // can be any digital pin
#define XP 7   // can be any digital pin

// The display uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9

// Define Display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Define TouchScreen
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Game global object with initial state set to 0, game_mode set to 0 (since we haven't selected a mode yet)
Game battleship = Game(0, 0);

// Define grid box size (40x40)
#define BOXSIZE 40

void setup() {
  init();
  Serial.begin(9600);
  Serial.println("Welcome to Battleship!");

  tft.begin();
  // Draw menu
  draw_menu(tft);
}

void play_game(){
  // Calibrate minimum pressure to be considered a touch
  #define MINPRESSURE 10

  while(1){

    // Get a point and map it to the screen dimensions
    TSPoint p = get_point(tft, ts);

    // If the point has not enough pressure, repeat the loop (no press registers)
    if (p.z < MINPRESSURE) {
     continue;
    }
    int mode;
    switch (battleship.get_state()) {

      case 0:
        // Check what mode we are entering and update the battleship's game mode
        battleship.update_game_mode(tft, p);
        // If we make it down here, it means a screen press has been registered, update the game state.
        battleship.update_state(1);
        break;

      case 1:
        Serial.println("here");
        delay(1000);
        // Wait for player to set up all ships
        break;

      case 2:
        // Wait for player to lock in response
        break;

      case 3:
        // Wait for opponent to lock in response
        break;

      case 4:
        // Show final game state
        break;
    }
  }
}

int main(){
  setup();
  play_game();
}
