#include "game.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen

// Game object constructor
Game::Game(int game_state, int game_mode){
  _state = game_state;
  _mode = game_mode;
  _player_isalive = 1;  // 1: is alive
}

// Returns the state of the game
int Game::get_state(){
  return _state;
}

// Returns the game mode
int Game::get_mode(){
  return _mode;
}

// Check how many players are selected and go to appropriate game mode
// Returns 1 if single player is selected, returns 2 if 2 player is selected
int Game::update_game_mode(Adafruit_ILI9341 display, TSPoint point){
  if (point.y < 120){
    // 1 player mode is selected
    _mode = 1;
  }
  else if (point.y > 120){
    // 2 player mode is selected
    _mode = 2;
  }
  else{
    // We should never get here
    _mode = 0;
  }
}

// Updates the state of the game
void Game::update_state(int new_state){
  _state = new_state;
}

// Updates the player alive status
// Status codes: 0: dead, 1: alive, 2: tie
void Game::update_is_alive(int new_status){
  _player_isalive = new_status;
}

// Returns the player alive status
int Game::get_is_alive(){
  return _player_isalive;
}
