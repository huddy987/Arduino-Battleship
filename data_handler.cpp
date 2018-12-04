#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
#include "block.h"    // Block class
#include "game.h"     // Game class
#include "data_handler.h"

// (*(play_arr + #)) --> this is the block object
/*
               A   B    C     D    E   F    G
      -----------------------------------------
      |  M  | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 5
      |     |----------------------------------|
      |  E  | 28 | 29 | 30 | 32 | 32 | 33 | 34 | 4
      |     |----------------------------------|
      |  N  | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 3
      |     |----------------------------------|
      |  U  | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 2
      |     |----------------------------------|
      |  "  | 07 | 08 | 09 | 10 | 11 | 12 | 13 | 1
      |     |----------------------------------|
      |  "  | 00 | 01 | 02 | 03 | 04 | 05 | 06 | 0
      -----------------------------------------
*/


/*
const int gen_pin = 1;

void setup() {
    init();
    pinMode(gen_pin, INPUT);
    Serial.begin(9600);
    Serial3.begin(9600);
    Serial.flush();
    Serial3.flush();
}
*/


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
    8 = enemy's: has boat but not shot
  c. the array element will store a pointer to act as tuple.
    i. the first element of the tuple represents the ship id.
    ii. second element stores the block state
    iii. if all blocks of a ship has been hit, it will be sunk
*/

      /*
      Block()  // declares the constructor --> initiates all values as 0
      {
        boat_id = 0;
        block_state = 0;*/

/*
  uint8_t boat_id - the id of the boat that resides there
  uint8_t block_state - the state of my block
  uint8_t enemy_state - the enemy block state from my POV

*/

/*
  Inverse function of determine_block

  Inputs: grid_pos - A0 to G5
  Output: element - which block number it is (0 to 41)
*/
uint8_t determine_array_element(String grid_pos){
  //idk if I can index strings in C++ so this should be fun to test
  uint8_t column = static_cast<int>(grid_pos[0]) - 64;
  uint8_t row = static_cast<int>(grid_pos[1]) - 48;
  uint8_t element = column + (row * 7) -1;
  return element;
}

/*
  Inverse function of determine_array_element

  Inputs: block_number - 0 to 41 only
  Output: String block - A0 to G5
*/
String determine_block(uint8_t block_number) {
  String a,b;

  uint8_t column;
  for (uint8_t i=0; i<7 ; i++) {
    if (((block_number-i)%7)==0) {column = i; break;}
  }
  a = String((char(column + 65)));

  if ((0<=block_number)&&(block_number<=6)) {b = "0";}
  else if ((7<=block_number)&&(block_number<=13)) {b = "1";}
  else if ((14<=block_number)&&(block_number<=20)) {b = "2";}
  else if ((21<=block_number)&&(block_number<=27)) {b = "3";}
  else if ((28<=block_number)&&(block_number<=34)) {b = "4";}
  else if ((35<=block_number)&&(block_number<=41)) {b = "5";}
  else {Serial.println("Error in determine_block.");}

  // Just concatenate String a and String b here
  return ((a+b));
}


/*
  This following two function shows the game on serial-mon
  This one print's the player's own blocks
*/
void print_blocks(Block player_array[]){
  Serial.println();
  Serial.println("My Block States");
  for(int i=1; i<43; i++){
    if ((i%7)==0) {Serial.print((*(player_array + i - 1)).getBlock());Serial.println();}
    else {Serial.print((*(player_array + i - 1)).getBlock());}
  }
  Serial.println();
}

/*
  This function shows the game on serial-mon
  This one print's the player's enemy states
*/
void print_blocks_2(Block player_array[]){
  Serial.println();
  Serial.println("Enemy States");
  for(int i=1; i<43; i++){
    if ((i%7)==0) {Serial.print((*(player_array + i - 1)).getEnemy());Serial.println();}
    else {Serial.print((*(player_array + i - 1)).getEnemy());}
  }
  Serial.println();
}

void print_blocks_3(Block player_array[]){
  Serial.println();
  Serial.println("My Boat ID's");
  for(int i=1; i<43; i++){
    if ((i%7)==0) {Serial.print((*(player_array + i - 1)).getBoat());Serial.println();}
    else {Serial.print((*(player_array + i - 1)).getBoat());}
  }
  Serial.println();
}

void print_blocks_4(Block player_array[]){
  Serial.println();
  Serial.println("My Enemy's Boat ID's");
  for(int i=1; i<43; i++){
    if ((i%7)==0) {Serial.print((*(player_array + i - 1)).getEnemyBoat());Serial.println();}
    else {Serial.print((*(player_array + i - 1)).getEnemyBoat());}
  }
  Serial.println();
}



/*
Inputs: the player's array and the boat id
Output: true - if all blocks with the same boat id has been shot (==3)
        false - otherwise
*/
bool check_my_boat_sunk(Block play_arr[], uint8_t boat_id){
  uint8_t i, return_val;
  uint8_t boat_count = 0;
  uint8_t shot_count = 0;

  for(i=0;i<42;i++) {   /// count how many blocks has the same boat id
    if (play_arr[i].getBoat() == boat_id) {
      boat_count++;

    // if it's been shot and same boat id
    if (play_arr[i].getBlock() == 3) {
      shot_count++;
    }
  }

  if (boat_count == shot_count){
    return_val = true;
  } else {return_val = false;}
  }
  return return_val;
}

/*
This will transform all blocks with the same boat id into shot (==4)
Inputs: the player's array and the boat id
Output: the number of blocks that's been equated to 4 (died)
 */
void kill_my_boat(Block play_arr[], uint8_t boat_id){
  for(uint8_t i=0;i<42;i++) {
    if (((*(play_arr + i)).getBoat()) == boat_id) {
      (*(play_arr + i)).updateBlock(4);
    }
  }
}

/*
 * Use in conjuction with check_all_boat_sunk
 * If a boat is dead, make its state == 4
 */
void check_if_my_boat_sunk(Block play_arr[]){
  // iterate over the three boats hidden
  for (int i = 1; i < 4; i++) {
    if (check_my_boat_sunk(play_arr, i)) {
      kill_my_boat(play_arr, i);
      // Maybe Do Something else here too. Like print the dead boat or something
    }
  }
}



/*
Inputs: the player's array
        enemy_block_number - the block that the enemy wants to attack
Outputs: int, the new state of the block that the enemy shot
*/
uint8_t recieve_turn(Block play_arr[], uint8_t boat_block_number){
  switch (play_arr[boat_block_number].getBlock()){
    case 0:
      return 1;
    case 2:
    // Else, mark it as hit but not comletely sunk
        return 3;   // Else, return 6
  }
}


/*
Inputs: the player's array and the boat id
Output: true - if all blocks with the same boat id has been shot (==3)
        false - otherwise
*/
bool check_enemy_boat_sunk(Block play_arr[], uint8_t boat_id){
  uint8_t i, return_val;
  uint8_t boat_count = 0;
  uint8_t shot_count = 0;

  for(i=0;i<42;i++) {   /// count how many blocks has the same boat id
    if (play_arr[i].getEnemyBoat() == boat_id) {
      boat_count++;

    // if it's been shot and same boat id
    if (play_arr[i].getEnemy() == 6) {
      shot_count++;
    }
  }

  if (boat_count == shot_count){
    return_val = true;
  } else {return_val = false;}
  }
  return return_val;
}

/*
This will transform all blocks with the same boat id into shot (==4)
Inputs: the player's array and the boat id
Output: the number of blocks that's been equated to 4 (died)
 */
void kill_enemy_boat(Block play_arr[], uint8_t boat_id){
  for(uint8_t i=0;i<42;i++) {
    if (((*(play_arr + i)).getEnemyBoat()) == boat_id) {
      (*(play_arr + i)).updateEnemy(7);
    }
  }
}

/*
 * Use in conjuction with check_all_boat_sunk
 * If a boat is dead, make its state == 4
 */
void check_if_enemy_boat_sunk(Block play_arr[]){
  // iterate over the three boats hidden
  for (int i = 1; i < 4; i++) {
    if (check_enemy_boat_sunk(play_arr, i)) {
      kill_enemy_boat(play_arr, i);
      // Maybe Do Something else here too. Like print the dead boat or something
    }
  }
}

/*
Inputs: the player's array
        my_block_number - the block number that I shot
Outputs: the new block state of the block that I shot
*/
uint8_t update_my_array(Block play_arr[], uint8_t my_block_number){

  switch (play_arr[my_block_number].getEnemy()) {
    case 0:
      return 5;   // If there's no boat, return 5

    case 8:
      return 6;  // If there's a boat return 6 (boat has been hit)
  }
}

/*
Checks if we have lost, returns 1 if they have
*/
bool check_self_death(Block play_arr[], uint8_t blocks_allowed){
  uint8_t counter = 0;
  for(int i = 0; i < 42; i++){
    if(play_arr[i].getBlock() == 4){    // If all boats are sunk
        counter++;
        if(counter == blocks_allowed){
          return true;
        }
    }
  }
  return false;
}

/*
Checks if enemy has lost, returns 1 if they have
*/
bool check_enemy_death(Block play_arr[], uint8_t blocks_allowed){
  uint8_t counter = 0;
  for(int i = 0; i < 42; i++){
    if(play_arr[i].getEnemy() == 7){   // If all boats are sunk
        counter++;
        if(counter == blocks_allowed){
          return true;
        }
    }
  }
  return false;
}


// Check if you have lost or your enemy has lost, and set gamestate to 3 if it is
// Updates the game object to the appropriate alive status (using pointers)
// Returns 0 or 1 so we can either continue in the loop or not
// https://stackoverflow.com/questions/8095078/how-to-modify-a-struct-in-a-function-and-return-to-main
bool check_deaths(Block play_arr[], int squares_allowed, Game *game){

  // We have tied, skip game screen
  if (check_self_death(play_arr, squares_allowed) and check_enemy_death(play_arr, squares_allowed)){
    game->update_is_alive(2);
    game->update_state(3);
    return 1;
  }

  // We have lost, skip to end game screen
  else if (check_self_death(play_arr, squares_allowed)){
    game->update_is_alive(0);
    game->update_state(3);
    return 1;
  }

  // We have won, skip to end game screen
  else if (check_enemy_death(play_arr, squares_allowed)){
    game->update_is_alive(1);
    game->update_state(3);
    return 1;
  }
  return 0;
}

// Determines block state before it was shot
int determine_previous_state(Block play_arr[], uint8_t boat_block_number){
  switch (play_arr[boat_block_number].getBlock()){
    case 1:   // If it was a miss, return the undisturbed case
      return 0;
    case 3:
        return 2;   // If it was hit, return the not-hit state
    case 4:
      return 2;     // If boat was fully sunk, return the non-hit state
  }
}
