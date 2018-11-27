#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen

// Player, used for tracking tile specifics states

/* Test Code:
void play_game(){
  String self[5] = {"A0", "A1", "A2", "A3", "A4"};
  String other[5] = {"B0", "B1", "B2", "B3", "B4"};
  Player self_player = Player(squares_allowed, self, other);
  String *test = self_player.get_blocks();
  String *test2 = self_player.get_opponent_blocks();
}
*/

class Player{
  private:
    String _blocks[18]; // Set to the amount of blocks allowed
    String _enemy_blocks[18]; // Set to the amount of blocks allowed
    int _is_alive;
  public:
    // Object constructor
    Player(int blocks_allowed, String self_blocks[], String enemy_blocks[]);

    // Returns all of own's blocks (pointer)
    String *get_blocks();

    // Retuns all of opponent's blocks (pointer)
    String *get_opponent_blocks();

    // Updates player alive status
    void update_is_alive(int alive_status);

    // Updates the players chosen blocks
    void update_self_blocks(String new_blocks[], int blocks_allowed);

    // Checks if player is alive
    int check_is_alive();
};

#endif
