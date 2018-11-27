#include "player.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen


// Object constructor
Player::Player(int blocks_allowed, String self_blocks[], String enemy_blocks[]){
  // For however many blocks are allowed, append a block to both lists
  for(int i = 0; i < blocks_allowed; i++){
    _blocks[i] = self_blocks[i];
    _enemy_blocks[i] = enemy_blocks[i];
  }
  _is_alive = 1;  // Player should always be initially alive
}

// Updates the players chosen blocks
void Player::update_self_blocks(String new_blocks[], int blocks_allowed){
  for(int i = 0; i < blocks_allowed; i++){
    _blocks[i] = new_blocks[i];
  }
}

// Returns all of own's blocks (pointer)
String *Player::get_blocks(){
  return _blocks;
}

// Retuns all of opponent's blocks (pointer)
String *Player::get_opponent_blocks(){
  return _enemy_blocks;
}

// Updates player's alive status
void Player::update_is_alive(int alive_status){
  _is_alive = alive_status;
}

// Checks if player is alive
int Player::check_is_alive(){
  return _is_alive;
}
