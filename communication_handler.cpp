
#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
// include the data_handler here for updating arrays

//NOTE: remember to initialize Serial3 in the setup similar to diffie


// just converts the grid_position into the array element to make indexing easier
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
          0 otherwise
*/
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

// makes all parts of the boat with the same boat id = 4
// meaning the entire boat is dead
void kill_entire_boat(uint8_t play_arr[][2], uint8_t boat_id){
  uint8_t i;
  for(i=0;i<10;i++) {   /// 10 should be replaced
  if (**(play_arr + i) == boat_id) {
    (*(*(play_arr + i) + 1)) = 4;
  }
 }
}

void recieve_turn(uint8_t player_array[][2], uint8_t block_number){
  uint8_t boat_id;
 /*
  *(*(player_array + block_number)) == the boat ID
  *(*(player_array + block_number) + 1) == the block state
  */

  switch (*(*(player_array + block_number) + 1)) {
    case 0 :  // undisturbed
      *(*(player_array + block_number) + 1) = 1;
      Serial3.write(5); // tell them that shot but no boat;
      break;

    case 2 :  // boat hidden; not shot
      *(*(player_array + block_number) + 1) = 3;
      boat_id = *(*(player_array + block_number));

      //if all boat dead, the if-statement will execute
      if (check_all_boat_sunk(player_array, boat_id)){
        // kill their entire boat
        kill_entire_boat(player_array, boat_id);
        Serial3.write(7);
      } else {Serial3.write(6);}
      break;
    default :
      Serial.print("Error");
}}


void making_a_turn(String grid_pos,uint8_t player_array[][2]){
  uint8_t block_number = determine_array_element(grid_pos); // change the A5 here
  Serial3.write(block_number);   // send your block number
  while (Serial3.available() < 7){ }   // its just gonna wait until then
    // this is the bock that the enemy has selected
  uint8_t enemy_block_number = Serial3.read(); // recieve their block#
  recieve_turn(player_array, enemy_block_number);
}


/*

void setup() {
  init();
  Serial.begin(9600);
  Serial.println("Welcome to Battleship!");
}

int main(){
  //uint8_t player_array[42][2];

  setup();
  while (1){

    uint8_t block_number = determine_array_element("A5");
    Serial.println(block_number);
    if (Serial3.available() > 0){
      uint8_t enemy_block = Serial3.read();
      Serial.println(enemy_block);
    }


  }
  return 0;
}

  if my_turn(my_array)
  if enemy_turn(enemy_array)
  ///put this in main file

*/
