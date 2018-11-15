#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
/// TODO: EVERYTHING IS UPDATED ON COMMUNICATION HANDLER. NOT THIS.

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


/*
 * converts the grid position as given by a string grid_pos
 * and returns the block's index in the player's array
*/

uint8_t determine_array_element(String grid_pos){
  //idk if I can index strings in C++ so this should be fun to test
  uint8_t column = static_cast<int>(grid_pos[0]) - 64;
  uint8_t row = static_cast<int>(grid_pos[1]) - 48;
  uint8_t element = column + (row * 7) -1;
  return element;
}


/*
  Input: player array and the boat id
  Output: 1 if all parts of the boat has been shot
              --- if all blocks with the same boat id has been shot
          0 otherwise
*/ // THIS WORKS
uint8_t check_all_boat_sunk(uint8_t play_arr[][2], uint8_t boat_id){
  uint8_t i, return_val;
  uint8_t boat_count = 0;
  uint8_t shot_count = 0;

  for(i=0;i<42;i++) {   /// idk if this should be 41 or 42
    if (**(play_arr + i) == boat_id) {
      boat_count++;
    }
    // if it's been shot but full boat isnt dead
    if (*(*(play_arr + i) + 1) == 3) {
      shot_count++;
    }
  if (boat_count == shot_count){
    return_val = 1;
  } else {return_val = 0;}
  }
  return return_val;
}

/*
  Input: player array and the boat id
  Output: no output but it updates the player's array
          -- all blocks with the same boat id will
              be "dead" (hold a state of 4). entire boat is dead
*/ // THIS WORKS
void kill_entire_boat(uint8_t play_arr[][2], uint8_t boat_id){
  uint8_t i;
  for(i=0;i<42;i++) {   /// TODO: check here later
  if (**(play_arr + i) == boat_id) {
    (*(*(play_arr + i) + 1)) = 4;
  }
 }
}

/* re
 reads which block the enemy entered and updates the game array accordingly
// if theblock is undisturbed (0) it should go to 1 (shot, no boat)
// if there is a hidden boat (2), send to 3 (if full boat not dead)
//                                send to 4 (if full boat dead)
*/

/*
Input: player array and the what block the enemy entered
Output: no output but it updates the player's array
      will check what the state of that block is.
        -- if 0 (no boat) --> becomes 1 (shot, but no boat)
        -- if 2 (hidden boat) --> becomes 3
            -->checks if all blocks with the same boat id has been shot,
                if true, all of the said blocks --> 4 (full boat sunk)
*/
void recieve_turn(uint8_t player_array[][2], uint8_t enemy_block_number){
  uint8_t boat_id;
 /*
  *(*(player_array + block_number)) == the boat ID
  *(*(player_array + block_number) + 1) == the block state
  */

  switch (*(*(player_array + enemy_block_number) + 1)) {
    case 0 :  // undisturbed
      *(*(player_array + enemy_block_number) + 1) = 1;
      Serial3.write(5); // tell them that shot but no boat;
      break;

    case 2 :  // boat hidden; not shot
      *(*(player_array + enemy_block_number) + 1) = 3;
      boat_id = *(*(player_array + enemy_block_number));

      //if all boat dead, the if-statement will execute
      if (check_all_boat_sunk(player_array, boat_id)){
        // kill their entire boat with the same boat id
        kill_entire_boat(player_array, boat_id);
        Serial3.write(7);
      } else {Serial3.write(6);}
      break;
    default :
      Serial.print("Error");
  }
}

/*
  Input: player array and the boat id
  Output: no output but it updates the player's array
          -- all blocks with the same boat id will
              be 7 (enemy's full boat sunk). entire boat is dead
*/
void kill_entire_enemy_boat(uint8_t play_arr[][2], uint8_t boat_id){
  uint8_t i;
  for(i=0;i<42;i++) {   /// TODO: check here later
  if (**(play_arr + i) == boat_id) {
    (*(*(play_arr + i) + 1)) = 7;
  }
 }
}

/*
  Input: player array and my_block_number (the block number I picked)
  Output: no output but it updates the player's array
        if the enemy sent me a
          -- 5 - I shot there, but there was no boat
          -- 6 - shot there and there was a boat
          -- 7 - shot, yes boat, and the entire boat has been sunk
               - if this case, make all blocks with the same boat id = 7
*/

void update_my_array(uint8_t player_array[][2], uint8_t my_block_number){
  uint8_t boat_id;

  // its just gonna wait until the other arduino sends their response
  while (Serial3.available() < 7){ };
  uint8_t enemy_response = Serial3.read();
  switch (enemy_response) {
    case 5:  // you shot nothing
    *(*(player_array + my_block_number) + 1) = 5;
      break;

    case 6 :  // you shot something, but boat didn't die
      *(*(player_array + my_block_number) + 1) = 6;
      break;

    case 7 :  // you shot the boat and all parts of it has been shot.
      boat_id = *(*(player_array + my_block_number));

      // make all blocks with the same boat id (block state = 7)
      kill_entire_enemy_boat(player_array, boat_id);

      break;
    default :
      Serial.println("Error in update_my_array");
  }
}

// Input: the grid_pos - the grid position that the player entered
//        player_array - the payer's array that contains the state of every block
void making_a_turn(String grid_pos,uint8_t player_array[][2]){
  uint8_t my_block_number = determine_array_element(grid_pos); // change the A5 here
  Serial3.write(my_block_number);   // send your block number
  while (Serial3.available() < 7){ }   // its just gonna wait until then
    // this is the bock that the enemy has selected
  uint8_t enemy_block_number = Serial3.read(); // recieve their block#
  recieve_turn(player_array, enemy_block_number);
  update_my_array(player_array, my_block_number);
}
