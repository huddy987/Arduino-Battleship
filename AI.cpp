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

// Return AI-selected tiles
String *ai_pick_positions() {
  // 5 + 4 + 3 boats makes 12 tiles
  String *opponent_ships = new String[12];

  // Grid box constants
  char let[7] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
  char num[6] = {'0', '1', '2', '3', '4', '5'};

  // TODO: pick a boat of length 5

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
String *ai_take_shot(Block game_arr[]) {
  // only 1 selection will be made
  String *shot_position = new String[1];

  // Grid box constants
  char let[7] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
  char num[6] = {'0', '1', '2', '3', '4', '5'};

  // TODO: Make tile picking more robust

  // Continually pick tiles until we pick a valid one
  while (true){
    shot_position[0] = String(let[random(0, 7)]) + String(num[random(0, 6)]);

    // If the chosen block is either 0 (undisturbed) or 2 (boat hidden) take the shot
    if (game_arr[determine_array_element(shot_position[0])].getBlock() == 0 ||
        game_arr[determine_array_element(shot_position[0])].getBlock() == 2) {
          break;
        }
  }

  return shot_position;
}
