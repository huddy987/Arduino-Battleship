#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
#include "block.h"    // Block class
#include "game.h"     // Game class
#include "data_handler.h"
#include "game.h"     // Game class
#include "boat_id.h"

int determine_first_block_type(String grid_pos){
  int block_number =  determine_array_element(grid_pos);

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
   // make a class later
int valid_third_block(String grid_pos[], int squares_selected) {
  // calculate response from one and two here again.
  int holder1 = 0;
  if (squares_selected == 8)  {holder1 = 5;}
  else if (squares_selected == 12)  {holder1 = 9;}

  int block1_number =  determine_array_element(grid_pos[holder1]);
  int block2_number =  determine_array_element(grid_pos[holder1 + 1]);
  int block3_number =  determine_array_element(grid_pos[holder1 + 2]);
  int block_type = valid_second_block(grid_pos[holder1], grid_pos[holder1 + 1]);

  // Iterate over every block that was entered: Clean up later.
  int difference1 = block3_number - block1_number;  // for use later
  int difference2 = block3_number - block2_number;
  // int seven[2] = {7, -7};
  // int one[2] = {1, -1};

  // if block type = +/- 7 ==> there should be atleast one that's  +/-
  if (block_type == -7 or block_type == 7) {
      if (difference1 == -7 or  difference1 == 7 or
          difference2 == -7 or  difference2 == 7) {return 1;} 
      else {return 0;}
  } else if (block_type == -1 or block_type == 1) {
      if (difference1 == -1 or  difference1 == 1 or
          difference2 == -1 or  difference2 == 1) {return 1;} 
      else {return 0;}
  }
}
*/

int special_case(int block_numbers[], int cardinal) {
  int return_val = 1;
  int index;

  // for the upper bound wraparound
  for (int i=0; i<(cardinal-1); i++) {
    if (block_numbers[i] % 7 == 0) {index = i; break;}
  }
  if (block_numbers[index] - block_numbers[cardinal-1] == 1) {return 0;}

  // for the lower bound wraparound
  for (int i=0; i<(cardinal-1); i++) {
    if ( (block_numbers[i] + 1) % 7 == 0) {index = i; break;}
  }
  if (block_numbers[cardinal-1] - block_numbers[index] == 1) {return 0;}

  return return_val;
}


int valid_consequent_blocks(String grid_pos[], int squares_selected, int cardinal) {
  int holder1 = 0;
  if (squares_selected == 8 or squares_selected == 9)  {holder1 = 5;}
  else if (squares_selected == 12)  {holder1 = 9;}

  // make the array of the block numbers
  int block_numbers[cardinal];
  for (int i=0; i<cardinal; i++) {
    block_numbers[i] = determine_array_element(grid_pos[holder1 + i]);
  }


  // array of differences
  int differences[cardinal - 1];
  for (int i=0; i<(cardinal-1); i++) {
    differences[i] = block_numbers[cardinal - 1] - block_numbers[i];
  }

  int block_type = valid_second_block(grid_pos[holder1], grid_pos[holder1 + 1]);

  if (abs(block_type) == 7) {
    for (int i=0; i<(cardinal-1); i++){
      if (abs(differences[i]) == 7) {Serial.println("We here."); return 1;}
    }
  } else if (abs(block_type) == 1) {   /// do for special 
    for (int i=0; i<(cardinal-1); i++){
      if (abs(differences[i]) == 1) {
        Serial.println("We here69.");
        return(special_case(block_numbers, cardinal));}
    }
  }
  Serial.println("We here2.");
  return 0;
}

/* This function assumes a 12 boat inputs with 5,4,3 blocks in that order */
int first_contact(String block_arr[], int squares_selected, int squares_allowed){
  // idk if a switch statement is better here

  int holder = 0;
  // this handles block number 1 and two for the three boats
  if (squares_selected == 6 or squares_selected == 7) {holder = 5;}
  else if (squares_selected == 10 or squares_selected == 11) {holder = 9;}



  switch (squares_selected) { 

    // first block of the boat
    case 1:
    case 6:
    case 10:
      // Serial.println("First block of the boat: ");
      return(determine_first_block_type(block_arr[holder]));

    // second block of the boat
    case 2:
    case 7:
    case 11:
      // Serial.println("Second block of the boat");
      return(valid_second_block(block_arr[holder], block_arr[holder + 1]));

    // third block of the boat
    case 3:
    case 8:
    case 12:
      // Serial.println("Third block of the boat");
      return(valid_consequent_blocks(block_arr,squares_selected, 3));

    // fourth block of the boat
    case 4:
    case 9:
      // Serial.println("Fourth block of the boat");
      return(valid_consequent_blocks(block_arr,squares_selected, 4));
      // return(valid_fourth_block(block_arr, squares_selected));

    // fifth block of the boat
    case 5:
      // Serial.println("Fifth block of the boat");
      return(valid_consequent_blocks(block_arr,squares_selected, 5));
      // return(valid_fifth_block(block_arr, squares_selected));

    default:
      Serial.println("Somethings wrong in first_contact.");
      return 0;
  }
}


// assigns the boat ID's
void input_boat_id(String selected_array[], Block game_arr[]) {

  // iterate over the array of inputted blocks
  for (int i = 0; i < 12; i++) {

    // first boat
    if ((0<=i) && (i<=4)) {
      game_arr[determine_array_element(selected_array[i])].updateBoat(1);
    }

    // second boat
    else if ((5 <= i) && (i <= 8)) {
      game_arr[determine_array_element(selected_array[i])].updateBoat(2);
    }

    // third boat
    else if ((9 <= i) && ( i <= 11)) {
      game_arr[determine_array_element(selected_array[i])].updateBoat(3);
    } 

    // Should never execute
    else {Serial.println("Error in input_boat_id.");}
  }
}

void input_enemy_boat_id(String selected_array[], Block game_arr[]) {

  // iterate over the array of inputted blocks
  for (int i = 0; i < 12; i++) {

    // first boat
    if ((0<=i) && (i<=4)) {
      game_arr[determine_array_element(selected_array[i])].updateEnemyBoat(1);
    }

    // second boat
    else if ((5 <= i) && (i <= 8)) {
      game_arr[determine_array_element(selected_array[i])].updateEnemyBoat(2);
    }

    // third boat
    else if ((9 <= i) && ( i <= 11)) {
      game_arr[determine_array_element(selected_array[i])].updateEnemyBoat(3);
    } 

    // Should never execute
    else {Serial.println("Error in input_boat_id.");}
  }
}

/*  
 *  
 */ 
void freeze_boat(String selected_array[], String frozen_boats[],  int squares_selected)  {
  
  switch (squares_selected) {
    // first boat has been fully selected
    case 5:
      Serial.println("freeze_boat #1");
      for (int i=0; i<5; i++) {
        frozen_boats[i] = selected_array[i];
        Serial.println(frozen_boats[i]);
      }
      break;

    // second
    case 9:
      Serial.println("freeze_boat #2");
      for (int i=5; i<9; i++) {
        frozen_boats[i] = selected_array[i];
        Serial.println(frozen_boats[i]);
    }
    break;
  }
}

// if a block has not been frozen, return true
// if it's been frozen, return false.
bool check_not_frozen(String frozen_boats[], String pos, int squares_allowed) {
  // iterate over frozen boats and check if pos is anywhere in it;
  for (int i=0; i<squares_allowed; i++) {
    if (pos == frozen_boats[i]) {
      Serial.println("block is frozen");
      return false;
    }
  }
  Serial.println("block is not frozen");
  return true;
}