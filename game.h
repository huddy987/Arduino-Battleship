#ifndef _GAME_H_
#define _GAME_H_
#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen

// Game class, used for tracking current game state

/*
State codes:

0 - main menu
1 - setting up ships
2 - playing turn
3 - end game screen
*/

class Game{
  private:
    int _state;
    int _mode;
  public:
    // Object constructor
    Game(int user_state, int game_mode);

    // Returns the game mode
    int get_mode();

    // Checks the game mode (when at main menu)
    int update_game_mode(Adafruit_ILI9341 display, TSPoint point);

    // Returns the current game state
    int get_state();

    // Updates the current game state
    void update_state(int new_state);
};

#endif
