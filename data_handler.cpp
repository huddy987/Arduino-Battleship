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
  //idk if I can index strings in C++ so this should be fun to test
  uint16_t column = static_cast<int>(grid_pos[0]) - 64;
  uint16_t row = static_cast<int>(grid_pos[1]) - 48;
  uint16_t element = column + (row * 7) -1;
  return element;
  }

/* if all parts of the boat with boat_id has been shot,
  this function will return a 1. otherwise, a 0.
*/
uint16_t check_boat(uint16_t player_array[][2], uint16_t boat_id){
  for(int i=0;i<41;i++) {
    if (**(player_array + i) == boat_id){
      uint16_t block_state = *(*(player_array + i) + 1);
      if ((block_state = 2 or  ))
    }
  }
}




// the input of this function will be the array and the block number
// I imagine this being called when the player makes a turn.
void recieve_turn(uint16_t player_array[][2], uint16_t block_number){
  /*
  *(*(player_array + block_number)) == the boat ID
  *(*(player_array + block_number) + 1) == the block state */

  switch (*(*(player_array + block_number) + 1)) {
    case 0 :  // undisturbed
      *(*(player_array + block_number) + 1) = 1;
      // Serial3.write a 5
      break;
    case 2 :  // boat hidden; not shot
      *(*(player_array + block_number) + 1) = 3;
      // TODO: check if all parts of this boat has been shot;
              // if yes this should be  = 4; Serial3.write a 7
      // else Serial3.write a 6
      break;
    default :
      Serial.print("Error");
  }

void after_enemy_update(uint16_t player_array[][2], uint16_t block_number){
  // Serial3.read what they sent.
  // uint16_t enemy_response = Serial3.read()
  switch (*(*(player_array + block_number) + 1)) {
    case 5:  // undisturbed
      *(*(player_array + block_number) + 1) = 5;
      // make the block a 5 on the enemy's side by serial3.write
      break;
    case 6:  // boat hidden; not shot
      *(*(player_array + block_number) + 1) = 6;
      // check if all parts of this boat has been shot; if yes = 4
      /// make the block a 6 on the enemy's side
      break;
    case 7:
    // look for the other parts of this boat and make them 7
    // do this by iterating over 2d array: if same boat id: make 7
    /*
    for (i=0;i<41;i++) {
    if **(player_array + i) == boat id of
  }
    */
    break;
    default : //Optional
      Serial.print("Error");
}

/*
void update_game_array(uint16_t player_array[][2], uint16_t block_number){
    *(*(player_array + block_number)) == the boat ID
    *(*(player_array + block_number) + 1) == the block state
    // *(*(arramina + 3)+1); 4th row, 2nd column

    switch(*(*(player_array + block_number) + 1)) {
      case 0 :  // undisturbed
        *(*(player_array + block_number) + 1;
        break;
      case 1 :  // shot but no boat
        statement(s);
        break;
      case 2 :  // boat hidden; not shot
        statement(s);
        break;
      case 3 :  // shot boat
        statement(s);
        break;
      case 4 :  // full boat sunk -- for graphics
        statement(s);
        break;
      case 5 :  // enemy's: shot but no boat
        statement(s);
        break;
      case 6 :  // enemy's: shot and yes boat
        statement(s);
        break;
      case 7 :  // enemy's: full boat sunk
        statement(s);
        break;
     default : //Optional
        Serial.print("Error");
  }
} */


void update_game_array(uint16_t player_array[][2], uint16_t block_number){
  /*
  *(*(player_array + block_number)) == the boat ID
  *(*(player_array + block_number) + 1) == the block state */
  // *(*(arramina + 3)+1); 4th row, 2nd column

  switch(*(*(player_array + block_number) + 1)) {
    case 0 :  // undisturbed
      *(*(player_array + block_number) + 1;
      break;
    case 1 :  // shot but no boat
      statement(s);
      break;
    case 2 :  // boat hidden; not shot
      statement(s);
      break;
    case 3 :  // shot boat
      statement(s);
      break;
    case 4 :  // full boat sunk -- for graphics
      statement(s);
      break;
    case 5 :  // enemy's: shot but no boat
      statement(s);
      break;
    case 6 :  // enemy's: shot and yes boat
      statement(s);
      break;
    case 7 :  // enemy's: full boat sunk
      statement(s);
      break;
   default : //Optional
      Serial.print("Error");
    }
}
