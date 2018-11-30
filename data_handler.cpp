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
  Inputs: String block - A0 to G5
  Output:  uint8_t number corresponding to where the block is:
    1. Top row
    2. Bottom row
    3. Left column
    4. Right column
    5. Centre tile

    Corner tiles will return negative values
    they have two constraints and will be especially dealt with
    -1 : top left (35)
    -2 : top right (41)
    -3 : bottom left (0)
    -4 : bottom right (6)
*/  // haven't checked this function yet
uint8_t determine_first_block_type(String grid_pos){
  uint8_t block_number =  determine_array_element(grid_pos);

  if (block_number == 35) {return (-1);}
  else if (block_number == 41) {return (-2);}
  else if (block_number == 0) {return (-3);}
  else if (block_number == 6) {return (-4);}

  if ((36<=block_number)&&(block_number<=40)) {return 1;}  // top row
  else if ((1<=block_number)&&(block_number<=5)) {return 2;}  // bottom row
  else if ((block_number%7)==0) {return 3;}  // left column
  else if (((block_number+1)%7)==0) {return 4;} // right column
  else {return 5;}
}

/*
  Important: Its the second block minus the first one
  returns true if the block selected is allowed

  Inputs: String grid_pos1 - the first block selected
          String grid_pos2 - the second block selected
          block_type - result of determine_first_block_type(1st block selected)
  Output: 0 - if the block is invalid
          if valid, returns (block2_number - block1_number)
*/
int valid_second_block(String grid_pos1, String grid_pos2) {
  int first_block_type = determine_first_block_type(grid_pos1);
  int block1_number =  determine_array_element(grid_pos1);
  int block2_number =  determine_array_element(grid_pos2);
  int difference = block2_number - block1_number;  // for use later
  int return_val = 0;

  switch (first_block_type) {

    // CORNER CASE
    case -1: // top Left
      if (difference == 1) {return_val = 1;}  // they selected 36
      else if (difference == -7) {return_val = -7;}  // they selected 28
      else {return_val = 0;}   // invalid block
      break;

    case -2:  // top right
      if (difference == -1) {return_val = -1;}  // they selected 40
      else if (difference == -7) {return_val = -7;}  // they selected 34
      else {return_val = 0;}   // invalid block
      break;

    case -3:  // bottom left
      if (difference == 7) {return_val = 7;}  // they selected 7
      else if (difference == 1) {return_val = 1;}  // they selected 34
      else {return_val = 0;}   // invalid block
      break;

    case -4:  // bottom right
      if (difference == -1) {return_val = -1;}  // they selected 7
      else if (difference == 7) {return_val = 7;}  // they selected 34
      else {return_val = 0;}   // invalid block
      break;


    // SIDE CASES
    case  1:  // top row
      if (difference == -1) {return_val = -1;}  // go left
      else if (difference == 1) {return_val = 1;}  // go right
      else if (difference == -7) {return_val = -7;}  // go down
      else {return_val = 0;}   // invalid block
      break;

    case  2:  // bottom row
      if (difference == -1) {return_val = -1;}  // go left
      else if (difference == 1) {return_val = 1;}  // go right
      else if (difference == 7) {return_val = 7;}  // go up
      else {return_val = 0;}   // invalid block
      break;

    case  3:  // left column
      if (difference == 1) {return_val = 1;}  // go right
      else if (difference == 7) {return_val = 7;}  // go up
      else if (difference == -7) {return_val = -7;}  // go up
      else {return_val = 0;}   // invalid block
      break;

    case  4:  // right column
      if (difference == -1) {return_val = -1;}  // go right
      else if (difference == 7) {return_val = 7;}  // go up
      else if (difference == -7) {return_val = -7;}  // go up
      else {return_val = 0;}   // invalid block
      break;

    // CENTRE CASE
    case  5:  // centre tiles
      if (difference == -1) {return_val = -1;}  // go left
      else if (difference == 1) {return_val = 1;}  // go right
      else if (difference == 7) {return_val = 7;}  // go up
      else if (difference == -7) {return_val = -7;}  // go up
      else {return_val = 0;}   // invalid block
      break;
  }
  return return_val;
}

/*
  Important: Its the second block minus the first one
  returns true if the block selected is allowed

  Inputs: String grids[] - maybe the array of
          String grid_pos - the block selected
          block_type - result of valid_second_block
  Output: false - invalid block
          true  - valid block
*/   // make a class later
bool valid_third_block(String grid_pos1, String grid_pos2, String grid_pos3, uint8_t block_type) {
  uint8_t block1_number =  determine_array_element(grid_pos1);
  uint8_t block2_number =  determine_array_element(grid_pos2);
  uint8_t block3_number =  determine_array_element(grid_pos3);

  // Iterate over every block that was entered: Clean up later.
  uint8_t difference1 = block3_number - block1_number;  // for use later
  uint8_t difference2 = block3_number - block2_number;

  // if block type = +/- 7 ==> there should be atleast one that's  +/- 7
  if (block_type == 7 or block_type == -7) {
    if (difference1 == 7 or difference1 == -7 or difference2 == 7 or difference2 == -7) {
      return true;
    } else {return false;}

  // if block type = +/- 1 ==> there should be atleast one that's  +/- 1
  } else if (block_type == 1 or block_type == -1){
    if (difference1 == 1 or difference1 == -1 or difference2 == 1 or difference2 == -1) {
      return true;
    } else {return false;}
  }
}

/*
  This following two function shows the game on serial-mon
  This one print's the player's own blocks
*/
void print_blocks(Block player_array[]){
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
  Serial.println("Enemy States");
  for(int i=1; i<43; i++){
    if ((i%7)==0) {Serial.print((*(player_array + i - 1)).getEnemy());Serial.println();}
    else {Serial.print((*(player_array + i - 1)).getEnemy());}
  }
  Serial.println();
}



/*
Inputs: the player's array and the boat id
Output: true - if all blocks with the same boat id has been shot (==3)
        false - otherwise
*/
bool check_all_boat_sunk(Block play_arr[], uint8_t boat_id){
  uint8_t i, return_val;
  uint8_t boat_count = 0;
  uint8_t shot_count = 0;

  for(i=0;i<42;i++) {   /// count how many blocks has the same boat id
    if (((*(play_arr + i)).getBoat()) == boat_id) {
      boat_count++;

    // if it's been shot and same boat id
    if (((*(play_arr + i)).getBlock()) == 3) {
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
uint8_t kill_entire_boat(Block play_arr[], uint8_t boat_id){
  uint8_t i;
  uint8_t count = 0;
  for(i=0;i<42;i++) {   /// TODO: check here later
  if (((*(play_arr + i)).getBoat()) == boat_id) {
    (*(play_arr + i)).updateBlock(4);
    count++;
  }
 }
 return count;
}

/*
This should send '7' followed by the amount of blocks that died.
then the block numbers of the blocks

Inputs: the player's array
        boat id  - id of the boat that died
        boat_death - how many blocks died
Output: none. for communication only

*/
void send_boat_death(Block play_arr[], uint8_t boat_id, uint8_t boat_death){
  // Serial3.write(7);
  // Serial3.write(boat_death);

  // Serial.print("Dying boats: "); Serial.println(boat_death);  // --> this is for debugging
  for(int i=0;i<42;i++) {
  if (((*(play_arr + i)).getBoat()) == boat_id) {
    if ((*(play_arr + i)).getBlock() == 4) {  // this embedded if statement should always be true
      // Serial3.print(i)  --> send the blocks that died

      // Serial.println(i);
    }
  }
 } //Serial.println();
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
    // If all parts of the boat have been hit, mark it as sunk
    /*
      if(check_all_boat_sunk(play_arr[], play_arr[boat_block_number.getBoat()])){
        return 4;   // If all part are hit, return 7
      }
      else{
      }
      */
    // Else, mark it as hit but not comletely sunk
        return 3;   // Else, return 6
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
    if(play_arr[i].getBlock() == 3 or play_arr[i].getBlock() == 4){
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
    if(play_arr[i].getEnemy() == 6 or play_arr[i].getEnemy() == 7){
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

/*
  This function might be extraneous but the logic is here

  1. send them which block you want to attack
  2. recieve which block they want to attack
  3. recieve_turn - see how their attack affects me
  4. update_my_array - see how my attack affected them

void making_a_turn(String grid_pos, Block player_array[]) {
  uint8_t my_block_number = determine_array_element(grid_pos);
  Serial3.write(my_block_number);

  uint8_t enemy_block_number = Serial3.read();
  recieve_turn(player_array, enemy_block_number);
  update_my_array(player_array, my_block_number);
} */




// Initialize the player's array by this.
// all 3 values of all blocks will == 0 initially
/*
Block game_arr[] = {Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),
    Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),
    Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),
    Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block()};
*/



/*  DEBUGGING
  This main is for debugging purposes only
  Everything under this is not needed for your purpose

int main() {
  setup();

  /// I could also implement a default constructor but for now,
  /// they all start with a value of 0
  // create an array of blocks will all have values of zero
  Block game_arr[] = {Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),
    Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),
    Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),
    Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block()};
    // --> idk how to put this in a for loop


    (game_arr[15]).block_state = 2;
    (game_arr[16]).block_state = 3;
    (game_arr[17]).block_state = 3;
    (game_arr[41]).block_state = 2;
    (game_arr[15]).boat_id = 90;
    (game_arr[16]).boat_id = 90;
    (game_arr[17]).boat_id = 90;
    (game_arr[41]).boat_id = 90;


    print_blocks(game_arr);
    print_boats(game_arr);
    recieve_turn(game_arr, 15);
    print_blocks(game_arr);
    print_boats(game_arr);
    recieve_turn(game_arr, 41);
    print_blocks(game_arr);
    print_boats(game_arr);
    update_my_array(game_arr, 8);
    print_blocks(game_arr);
    print_boats(game_arr);
    // Serial.println((game_arr[9]).getBoat);

    while (true) { }
        // if there are bytes in the user's arduino buffer
    Serial.end();
    return 0;
} */
