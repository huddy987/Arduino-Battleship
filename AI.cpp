#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h>  // Controller chip library
#include "TouchScreen.h"    // Library for TouchScreen
#include "./block.h"  // block class
#include "./touch_handler.h"  // touch handler header file
#include "./draw_handler.h"  // draw handler header file
#include "./game.h"  // game class
#include "./client.h"  // client class
#include "./data_handler.h"  // Block class and data handling functions
#include "./boat_id.h"

// Determine the blocks that should be selected after the first random selection
String ai_pick_block(String opponent_ships[]){
  return "test";
}

// Determine which direction to go in.
// LEGEND FOR DIRECTION NOTATION:

//  (ARDUINO BREADBOARD IS UP HERE)
//               0
//               ^
//               |
//       3 <  ---.---  > 1
//               |
//               v
//               2

int choose_direction(String root){
  if (root == "A0"){
    // Either go up or right
    return random(0, 1);
  }
  if (root == "A5"){
    // Either go down or right
    return random(1, 2);
  }
  if (root == "G5"){
    // Either go down or left
    return random(2, 3);
  }
  if (root == "G0"){
    // Either go up or left
    int selection = random(0, 1);
    if (selection == 0){
      return 0;
    }
    else{
      return 3;
    }
  }
  else{
    // We are somewhere not along the edges, pick any direction.
    return random(0, 3);
  }
}

// Return AI-selected tiles
String *ai_pick_boats() {
  // 5 + 4 + 3 boats makes 12 tiles
  String *opponent_ships = new String[12];

  // Grid box constants
  char let[7] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
  char num[6] = {'0', '1', '2', '3', '4', '5'};

  // Pick a boat of length 5
  // pick a "root" block
  opponent_ships[0] = String(let[random(0, 7)]) + String(num[random(0, 6)]);

  // pick a direction to go in based off of the root tile
  //int dir = choose_direction(opponent_ships[0]);

  // pick all other blocks based off of the root and direction
  for (int i = 1; i < 5; i++){
    opponent_ships[i] = ai_pick_block(opponent_ships);
  }

  // TODO: pick a boat of length 4

  // TODO: pick a boat of length 3

  // For now, hardcoded:
  opponent_ships[0] = String(let[0]) + String(num[0]);
  opponent_ships[1] = String(let[0]) + String(num[1]);
  opponent_ships[2] = String(let[0]) + String(num[2]);
  opponent_ships[3] = String(let[0]) + String(num[3]);
  opponent_ships[4] = String(let[0]) + String(num[4]);

  opponent_ships[5] = String(let[1]) + String(num[0]);
  opponent_ships[6] = String(let[2]) + String(num[0]);
  opponent_ships[7] = String(let[3]) + String(num[0]);
  opponent_ships[8] = String(let[4]) + String(num[0]);

  opponent_ships[9] = String(let[6]) + String(num[0]);
  opponent_ships[10] = String(let[6]) + String(num[1]);
  opponent_ships[11] = String(let[6]) + String(num[2]);

  return opponent_ships;
}

// Choose a position to shoot the enemy at.
String *ai_take_shot(Block game_arr[], String AI_last_shot, String root, int direction) {
  // only 1 selection will be made
  String *shot_position = new String[1];

  // Grid box constants
  char let[7] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
  char num[6] = {'0', '1', '2', '3', '4', '5'};

  // Fail safe counter if for some reason we loop in cases 0-4 for the root case
  int loop_flag = 0;

  // Continually pick tiles until we pick a valid one
  while (true) {

    // If there is no root, randomly choose a tile until a valid one is chosen.
    if (not root) {
      Serial.println("No root.");
      shot_position[0] = String(let[random(0, 7)]) + String(num[random(0, 6)]);

      // If the chosen block is either 0 (undisturbed) or 2 (boat hidden) take the shot
      if (game_arr[determine_array_element(shot_position[0])].getBlock() == 0 ||
          game_arr[determine_array_element(shot_position[0])].getBlock() == 2) {
            return shot_position;
      }
    }
    else {
      switch (direction){

        case 0:   // shoot up from the last shot
          Serial.println("Case 0");

          // Need this in case we recurse
          if(AI_last_shot[1] != '5'){

            // Find the index of the element in num and assign the shot to [let][num+1]
            for(int i = 0; i < 6; i++){
              if(AI_last_shot[1] == num[i]){
                shot_position[0] = AI_last_shot[0] + String(num[i+1]);
              }
            }

            // If the chosen block is either 0 (undisturbed) or 2 (boat hidden) take the shot
            if (game_arr[determine_array_element(shot_position[0])].getBlock() == 0 ||
                game_arr[determine_array_element(shot_position[0])].getBlock() == 2) {
                  return shot_position;
            }
          }

        case 1:   // shoot right from the last shot
          Serial.println("Case 1");

          // Need this in case we drop down from case 0 (skips if we have a G previous shot)
          if(AI_last_shot[0] != 'G'){

            // Find the index of the element in let and assign the shot to [let+1][num]
            for(int i = 0; i < 7; i++){
              if(AI_last_shot[0] == let[i]){
                shot_position[0] = String(let[i+1]) + AI_last_shot[1];
              }
            }

            // If the chosen block is either 0 (undisturbed) or 2 (boat hidden) take the shot
            if (game_arr[determine_array_element(shot_position[0])].getBlock() == 0 ||
                game_arr[determine_array_element(shot_position[0])].getBlock() == 2) {
                  return shot_position;
            }
          }

        case 2:   // shoot down from the last shot
          Serial.println("Case 2");

          // Need this in case we drop down from case 1 (skips if we have a 0 previous shot)
          if(AI_last_shot[1] != '0'){

            // Find the index of the element in let and assign the shot to [let][num - 1]
            for(int i = 0; i < 6; i++){
              if(AI_last_shot[1] == num[i]){
                shot_position[0] = AI_last_shot[0] + String(num[i-1]);
              }
            }

            // If the chosen block is either 0 (undisturbed) or 2 (boat hidden) take the shot
            if (game_arr[determine_array_element(shot_position[0])].getBlock() == 0 ||
                game_arr[determine_array_element(shot_position[0])].getBlock() == 2) {
                  return shot_position;
            }
          }

        case 3:   // shoot left from the last shot
          Serial.println("Case 3");

        // Need this in case we drop down from case 2 (skips if we have an A previous shot)
        if(AI_last_shot[0] != 'A'){

          // Find the index of the element in let and assign the shot to [let][num - 1]
          for(int i = 0; i < 7; i++){
            if(AI_last_shot[0] == let[i]){
              shot_position[0] = String(let[i-1]) + AI_last_shot[1];
            }
          }

          // If the chosen block is either 0 (undisturbed) or 2 (boat hidden) take the shot
          if (game_arr[determine_array_element(shot_position[0])].getBlock() == 0 ||
              game_arr[determine_array_element(shot_position[0])].getBlock() == 2) {
                return shot_position;
          }
        }
        if (loop_flag = 1){
          root = NULL;
        }
        // This is an edge case. Set direction to 0 and try again. Set flag to
        // 1 in case this fails and pick randomly (base case)
        direction = 0;
        loop_flag = 1;
      }
    }
  }

  return shot_position;
}
