#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
///

/* TODO:

0. each arduino will have an array from A5 to G0
  >>> element 0 is block A0, element 1 is B0... element 41 is block G5
    to keep track of which blocks have been selected
    keep this array in the stack of the main file in order
    make it permanent
1. make both arduinos store the blocks entered in both arduinos
  a. this will be done in the form of a 2D array
    - array of 42 elements (representing the blocks)
    - each of the 42 elements will contatain 2 integers:
      > block state
      > boat id
  b. a block can have 8 states.
    0 = undisturbed
    1 = has been shot but no boat
    2 = has a boat hidden; not shot
    3 = has a boat that's been shot
    4 = has a full boat sunk
    5 = enemy's: shot but no boat
    6 = enemy's: shot but yes boat
    7 = enemy's: full boat sunk
  c. the array element will store a pointer to act as tuple.
    i. the first element of the tuple represents the ship id.
    ii. second element stores the block state
    iii. if all blocks of a ship has been hit, it will be sunk
*/

uint16_t determine_array_element(String grid_pos){
  //idk if I can index strings in C++ so this should be fun
  uint16_t column = static_cast<int>(grid_pos[0]) - 64;
  uint16_t row = static_cast<int>(grid_pos[1]) - 48;
  uint16_t element = column + (row * 7) -1;
  return element
  }

// the input of this function will be the array, the block number
void update_game_array(int player_array[], uint16_t block_number){
  /*
  (*(player_array + block_number))[0] == the boat ID
  (*(player_array + block_number))[0] == the block state */
  if
}
