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
#include "./AI.h"

// Debug flag
bool debug = false;

// Super debug flag
bool super_debug = false;

// flag to turn off animations
bool animations = true;

// These are the four touchscreen analog pins
#define YP A10  // must be an analog pin, use "An" notation!
#define XM A11  // must be an analog pin, use "An" notation!
#define YM 6   // can be any digital pin
#define XP 7   // can be any digital pin

// The display uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9


// Define Display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Define TouchScreen
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Minimum pressure to be considered a touch
#define MINPRESSURE 10

// Game global object with initial state set to 0, game_mode set to 0
// (since we haven't selected a mode yet)
Game battleship = Game(0, 0);

// Client global object
Client client;

// Define grid box size (40x40)
int BOXSIZE = 40;

// Define how many squares are allowed
int squares_allowed = 12;

// Define block and initiate them
Block game_arr[] = {Block(), Block(), Block(), Block(),
    Block(), Block(), Block(), Block(), Block(), Block(),
    Block(), Block(), Block(), Block(), Block(), Block(),
    Block(), Block(), Block(), Block(), Block(), Block(),
    Block(), Block(), Block(), Block(), Block(), Block(),
    Block(), Block(), Block(), Block(), Block(), Block(),
    Block(), Block(), Block(), Block(), Block(), Block(),
    Block(), Block()};

// Set up arduino funcionality
void setup_arduino() {
  init();
  Serial.flush();   // Ensure there is no garbage in the serial buffer
  Serial3.flush();  // Ensure there is no garbage in the serial3 buffer
  Serial.begin(9600);
  Serial3.begin(9600);
  // Serial-mon message confirming that the arduinos are working properly
  Serial.println("Welcome to Battleship!");

  randomSeed(analogRead(0));  // set a random seed

  tft.begin();
  // Draw menu
  draw_menu_mode(tft);
}

// Resets arduino when called
// https://www.instructables.com/id/two-ways-to-reset-arduino-in-software/
void(* resetFunc) (void) = 0;  // declare reset function at address 0

// Handles main menu functionality
void main_menu(Adafruit_ILI9341 tft, int BOXSIZE) {
  while (true){
    // Get a point and map it to the screen dimensions
    TSPoint point = get_point(tft, ts);

    /*
      If the point doesn't have enough pressure,
      restart from the top (no press registers)
      If we are in the end game message phase (3),
      continue because we dont need presses anymore
    */
    if (point.z < MINPRESSURE && battleship.get_state() != 3) {
     continue;
    }

    // Check what mode we are entering and update the battleship's game mode
    battleship.update_game_mode(tft, point);
    if (debug == true){
      // Message indicating which game mode we are entering
      Serial.print(battleship.get_game_mode());
      Serial.println(" player mode was selected.");
    }

    if (battleship.get_game_mode() == 2){
      // Send "I am ready!" message to other arduino
      client.send_ready_message();

      // Wait for opponenet to respond
      client.wait(tft);
    }
    /*
     * If we make it down here, it means a screen press has been registered
     * on both arduinos (or just the 1 if in 1 player mode)
     * so we should update the game state
     */
    battleship.update_state(1);

    // Draw an empty map
    draw_empty_map(tft, BOXSIZE);

    // Draw a message telling the user to select 5 tiles
    draw_select(tft, BOXSIZE, "5");

    break;
  }
}

// Main control function for the game
void play_game() {

  // Variables for play and setup phase

  // Tracks how many squares are selected in both phases cases 1 and 2
  int squares_selected = 0;

  // Array containing the blocks selected (in "A0" notation)
  String selected[squares_allowed] = {};

  // Opponent block array
  String *opponent;

  // Flag for checking if a block has already been selected
  int already_selected = 0;

  // used for checking if block is valid input
  int block_is_allowed = 0;


  while (1) {
    // At the beginning of each loop, assume pressed block is not selected
    already_selected = 0;

    // Get a point and map it to the screen dimensions
    TSPoint point = get_point(tft, ts);

    /*
      If the point doesn't have enough pressure,
      restart from the top (no press registers)
      If we are in the end game message phase (3),
      continue because we dont need presses anymore
    */
    if (point.z < MINPRESSURE && battleship.get_state() != 3) {
     continue;
    }

    // Stores grid position in "A0" notation
    String pos = get_grid_position(point, BOXSIZE);


    switch (battleship.get_state()) {

      case 1:
      // This is the boat setup phase

        // If cancel is pressed, reset everything
        if (get_confirm_or_cancel(point) == 2) {  // If cancel is pressed
          squares_selected = 0;  // Reset the squares selected counter to 0

          // Draws board in a reset state
          clear_all_selections(tft, BOXSIZE, selected, squares_allowed);

          // Redraw grey confirm button
          draw_grey_setup(tft, BOXSIZE, squares_selected);

          // Replace selected squares in player's own array with 0
          for (int i = 0; i < squares_allowed; i++) {
            selected[i] = "";
          }
          continue;  // Restart the loop
        }

        // If confirm is pressed and not all tiles
        // are selected, ignore the press
        if (get_confirm_or_cancel(point) == 1 &&
          squares_selected < squares_allowed) {
          continue;
        }
        // If a block has already been selected restart the loop
        for (int i = 0; i < squares_allowed; i++) {
          if (pos == selected[i]) {
            if(debug == true){
              // Print the grid position if we are in debug mode
              Serial.println("Tile selected: " + selected[i]);
            }
            already_selected = 1;
            break;
          }
        }
        if (already_selected == 1) {
          delay(200);
          continue;
        }

        /* If squares selected is less than the amount of squares allowed,
           let the player choose another square. */

        if (squares_selected < squares_allowed) {
          // Redraw grey confirm button
          draw_grey_setup(tft, BOXSIZE, squares_selected);

          // This is where my boy hudson puts the blocks inside the array
          // Store the grid position in our array
          selected[squares_selected] =  pos;

          // will be zero if the block selected is
          // not adjacent to the previous blocks
          block_is_allowed = first_contact(selected, squares_selected + 1,
                                            squares_allowed);


          if (block_is_allowed) {
            if(debug == true){
              // Print the grid position if we are in debug mode
              Serial.println("Tile selected: " + selected[squares_selected]);
            }

            // Draw green so the user knows we've registered their press
            draw_at_grid_pos(tft, BOXSIZE, pos, ILI9341_GREEN);

            squares_selected++;

            // draw the grey setup tile that says how
            // many blocks the user should enter
            draw_grey_setup(tft, BOXSIZE, squares_selected);
          } else {
            // if block is not allowed remove it from the array of blocks
            selected[squares_selected] =  "";
          }

          // Restart loop (need to do it this way to
          // allow deselecting when there are 5 tiles)
          if (squares_selected == squares_allowed) {
            // Change menu to confirm button once all tiles are selected
            draw_green_confirm(tft, BOXSIZE);
          }
          delay(200);   // 200 ms delay to reduce accidental touches
          continue;

        } else {
            // when the user is done inputting blocks, input the boat ID's
            // print to serial-mon for confirmation
            input_boat_id(selected, game_arr, squares_allowed);

            if (super_debug == true){
              // Print self gamestate to serial-mon for debugging
              print_blocks_3(game_arr);
            }
        }

        // If confirm is not selected, restart the loop (wait)
        if (!(get_confirm_or_cancel(point) == 1)) {
          continue;
        }
        // Send "I am ready!" message to other arduino
        client.send_ready_message();

        // Wait for opponenet to respond
        client.wait(tft);

        // Send own selected tiles to opponent
        client.send_ships(selected, squares_allowed);

        delay(100);  // Small delay so that all the serial data can be sent

        // Receive opponent selected tiles
        // This is an array of strings of the enemy's ships
        opponent = client.receive_ships(squares_allowed);

        // assigns the boat IDs to my enemy's boat
        // print to serial-mon for confirmation
        input_enemy_boat_id(opponent, game_arr, squares_allowed);
        if (super_debug == true){
          // Print enemy gamestate to serial-mon for debugging
          print_blocks_4(game_arr);
        }

        // Update own blocks and enemy blocks
        for (int i = 0; i < squares_allowed; i++) {
          // Sets all your own blocks to hidden but not shot
          game_arr[determine_array_element(selected[i])].updateBlock(2);

          // Sets all your enemy's blocks to hidden but not shot
          game_arr[determine_array_element(opponent[i])].updateEnemy(8);
        }

        /* If we make it down here
         it means both arduinos have selected their tiles
         So we should update the gamestate
        */
        battleship.update_state(2);

        // Draw an empty map
        draw_empty_map(tft, BOXSIZE);

        // Reset our variables to 0 so we can reuse them in the next phase.
        squares_selected = 0;  // Reset the squares selected counter to 0
        Serial3.flush();  // Ensure there is no garbage in the serial3 buffer

        // Replace selected squares in player's own array with 0
        for (int i = 0; i < squares_allowed; i++) {
          selected[i] = "";
          opponent[i] = "";  // Remove the entry from the opponent's list
        }

        break;

      case 2:

        // If cancel is pressed, reset everything
        if (get_confirm_or_cancel(point) == 2) {  // If cancel is pressed
          squares_selected = 0;  // Reset the squares selected counter to 0

          // Draws board in a reset state
          clear_all_selections(tft, BOXSIZE, selected, 1);

          // Replace selected squares in player's own array with 0
          selected[0] = "";
          continue;  // Restart the loop
        }

        // If confirm is pressed before all tiles are selected, ignore the press
        if (get_confirm_or_cancel(point) == 1 && squares_selected < 1) {
          continue;
        }

        // If the block has already been selected, remove it from the list.
        if (pos == selected[0]) {
          if (pos == "") {   // If we get a mistouch, restart the loop
            if (debug == true) {
              Serial.println("Mistouch"); // confirm we had a mistouch
            }
            continue;
          }

          if (debug == true) {
            Serial.println("Tile deselected: " + selected[0]);  // print the block we selected
          }

          // Draw black so the user knows we've removed it
          draw_at_grid_pos(tft, BOXSIZE, selected[0], ILI9341_BLACK);

          // Draw a grey confirm button in case all were selected
          draw_grey_confirm(tft, BOXSIZE);
          squares_selected--;  // Reduce the counter by 1
          selected[0] = "";  // Remove the entry from our list
          delay(200);
          continue;
        }

        // If squares selected is less then the amount of squares allowed,
        // let the player choose another square.
        if (squares_selected < 1) {
          // If the block state is 0 (undisturbed) or 8 (enemy hidden boat)
          // allow the user to take a shot at that position
          if (game_arr[determine_array_element(pos)].getEnemy() == 0 ||
              game_arr[determine_array_element(pos)].getEnemy() == 8) {
            selected[0] =  pos;  // Store the grid position in our array
            if (debug == true) {
              Serial.println("Tile selected: " + selected[0]);  // print the block we selected
            }

            // Draw green so the user knows we've registered their press
            draw_at_grid_pos(tft, BOXSIZE, pos, ILI9341_GREEN);
            squares_selected++;

            // Restart loop (need to do it this way
            // to allow deselecting when there are 5 tiles)
            if (squares_selected == 1) {
              // Change menu to confirm button once all tiles are selected
              draw_green_confirm(tft, BOXSIZE);
            }
          }
          delay(200);  // 200 ms delay to reduce accidental touches
          continue;
        }

        // If confirm is not selected, restart the loop (wait)
        if (!(get_confirm_or_cancel(point) == 1)) {
          continue;
        }

        // Change menu to confirm button once all tiles are selected
        draw_green_confirm(tft, BOXSIZE);

        // Send "I am ready!" message to other arduino
        client.send_ready_message();

        // Wait for other arduino
        client.wait(tft);

        // clear serial3 buffer
        Serial3.flush();

        // Send own selected tile to opponent
        client.send_ships(selected, 1);

        delay(100);  // Small delay so that all the serial data can be sent

        // Receive a single block from opponent
        opponent = client.receive_ships(1);

        // Update enemy blocks with what you selected
        game_arr[determine_array_element(selected[
          0])].updateEnemy(update_my_array(game_arr,
            determine_array_element(selected[0])));

        // Update your own block with what your enemy shot
        game_arr[determine_array_element(opponent[
          0])].updateBlock(recieve_turn(game_arr,
            determine_array_element(opponent[0])));

        // Checks if any of my enemy's or my boats are dead
        // if yes, convert the states to display the proper colour
        check_if_enemy_boat_sunk(game_arr);
        check_if_my_boat_sunk(game_arr);

        if (super_debug == true){
          // Print the game states to serial-mon for debugging
          print_blocks_2(game_arr);
          delay(50);
          print_blocks(game_arr);
          delay(50);
        }

        // Check if you have lost or your enemy has lost
        // and set gamestate to 3 if it is
        if (check_deaths(game_arr, squares_allowed, &battleship)) {
          continue;   // If someone died, restart the loop
        }

        // If no one has lost, draw your own map (updated)
        draw_board_self(tft, BOXSIZE, game_arr, opponent, animations);

        // Wait until a touch is registered before continuing
        wait_for_touch(tft, ts, MINPRESSURE);

        // Show your opponents map (updated)
        draw_board_enemy(tft, BOXSIZE, game_arr, &selected[0], animations);

        // Reset our variables to 0 so we can reuse them in the next loop
        Serial3.flush();  // Ensure there is no garbage in the serial3 buffer
        squares_selected = 0;  // Reset the squares selected counter to 0
        selected[0] = "";
        opponent[0] = "";  // Remove the entry from the opponent's list
        break;

      case 3:
        // Draws outcome based off of what the alive status is
        draw_outcome(tft, battleship.get_is_alive());
        // Wait until a touch is registered before resetting the game
        wait_for_touch(tft, ts, MINPRESSURE);
        resetFunc();  // Resets the arduino
        break;
    }
  }
}

// Main control function for the solo game
void play_game_solo() {

  // Variables for play and setup phase

  // Tracks how many squares are selected in both phases cases 1 and 2
  int squares_selected = 0;

  // Array containing the blocks selected (in "A0" notation)
  String selected[squares_allowed] = {};

  // Opponent block array
  String *opponent;

  // AI memory of previous shot
  String AI_last_shot = *opponent;

  // AI memory of "root" shot
  String root = NULL;

  // AI direction of next shot
  int direction = NULL;

  // AI temporary direction
  int temp_direction = NULL;

  // Flag for checking if a block has already been selected
  int already_selected = 0;

  // used for checking if block is valid input
  int block_is_allowed = 0;


  while (1) {
    // At the beginning of each loop, assume pressed block is not selected
    already_selected = 0;

    // Get a point and map it to the screen dimensions
    TSPoint point = get_point(tft, ts);

    /*
      If the point doesn't have enough pressure,
      restart from the top (no press registers)
      If we are in the end game message phase (3),
      continue because we dont need presses anymore
    */
    if (point.z < MINPRESSURE && battleship.get_state() != 3) {
     continue;
    }

    // Stores grid position in "A0" notation
    String pos = get_grid_position(point, BOXSIZE);


    switch (battleship.get_state()) {

      case 1:
      // This is the boat setup phase

        // If cancel is pressed, reset everything
        if (get_confirm_or_cancel(point) == 2) {  // If cancel is pressed
          squares_selected = 0;  // Reset the squares selected counter to 0

          // Draws board in a reset state
          clear_all_selections(tft, BOXSIZE, selected, squares_allowed);

          // Redraw grey confirm button
          draw_grey_setup(tft, BOXSIZE, squares_selected);

          // Replace selected squares in player's own array with 0
          for (int i = 0; i < squares_allowed; i++) {
            selected[i] = "";
          }
          continue;  // Restart the loop
        }

        // If confirm is pressed and not all tiles
        // are selected, ignore the press
        if (get_confirm_or_cancel(point) == 1 &&
          squares_selected < squares_allowed) {
          continue;
        }
        // If a block has already been selected restart the loop
        for (int i = 0; i < squares_allowed; i++) {
          if (pos == selected[i]) {
            if(debug == true){
              // Print the grid position if we are in debug mode
              Serial.println("Tile selected: " + selected[i]);
            }
            already_selected = 1;
            break;
          }
        }
        if (already_selected == 1) {
          delay(200);
          continue;
        }

        /* If squares selected is less than the amount of squares allowed,
           let the player choose another square. */

        if (squares_selected < squares_allowed) {
          // Redraw grey confirm button
          draw_grey_setup(tft, BOXSIZE, squares_selected);

          // This is where my boy hudson puts the blocks inside the array
          // Store the grid position in our array
          selected[squares_selected] =  pos;

          // will be zero if the block selected is
          // not adjacent to the previous blocks
          block_is_allowed = first_contact(selected, squares_selected + 1,
                                            squares_allowed);


          if (block_is_allowed) {
            if(debug == true){
              // Print the grid position if we are in debug mode
              Serial.println("Tile selected: " + selected[squares_selected]);
            }

            // Draw green so the user knows we've registered their press
            draw_at_grid_pos(tft, BOXSIZE, pos, ILI9341_GREEN);

            squares_selected++;

            // draw the grey setup tile that says how
            // many blocks the user should enter
            draw_grey_setup(tft, BOXSIZE, squares_selected);
          } else {
            // if block is not allowed remove it from the array of blocks
            selected[squares_selected] =  "";
          }

          // Restart loop (need to do it this way to
          // allow deselecting when there are 5 tiles)
          if (squares_selected == squares_allowed) {
            // Change menu to confirm button once all tiles are selected
            draw_green_confirm(tft, BOXSIZE);
          }
          delay(200);   // 200 ms delay to reduce accidental touches
          continue;

        } else {
            // when the user is done inputting blocks, input the boat ID's
            // print to serial-mon for confirmation
            input_boat_id(selected, game_arr, squares_allowed);

            if (super_debug == true){
              // Print self gamestate to serial-mon for debugging
              print_blocks_3(game_arr);
            }
        }

        // If confirm is not selected, restart the loop (wait)
        if (!(get_confirm_or_cancel(point) == 1)) {
          continue;
        }

        // "Receive" boats from the AI
        opponent = ai_pick_boats();

        // assigns the boat IDs to my enemy's boat
        // print to serial-mon for confirmation
        input_enemy_boat_id(opponent, game_arr, squares_allowed);
        if (super_debug == true){
          // Print enemy gamestate to serial-mon for debugging
          print_blocks_4(game_arr);
        }

        // Update own blocks and enemy blocks
        for (int i = 0; i < squares_allowed; i++) {
          // Sets all your own blocks to hidden but not shot
          game_arr[determine_array_element(selected[i])].updateBlock(2);

          // Sets all your enemy's blocks to hidden but not shot
          game_arr[determine_array_element(opponent[i])].updateEnemy(8);
        }

        /* If we make it down here
         it means the arduino has selected its tiles
         So we should update the gamestate
        */
        battleship.update_state(2);

        // Draw an empty map
        draw_empty_map(tft, BOXSIZE);

        // Reset our variables to 0 so we can reuse them in the next phase.
        squares_selected = 0;  // Reset the squares selected counter to 0

        // Replace selected squares in player's own array with 0
        for (int i = 0; i < squares_allowed; i++) {
          selected[i] = "";
          opponent[i] = "";  // Remove the entry from the opponent's list
        }
        break;

      case 2:

        // If cancel is pressed, reset everything
        if (get_confirm_or_cancel(point) == 2) {  // If cancel is pressed
          squares_selected = 0;  // Reset the squares selected counter to 0

          // Draws board in a reset state
          clear_all_selections(tft, BOXSIZE, selected, 1);

          // Replace selected squares in player's own array with 0
          selected[0] = "";
          continue;  // Restart the loop
        }

        // If confirm is pressed before all tiles are selected, ignore the press
        if (get_confirm_or_cancel(point) == 1 && squares_selected < 1) {
          continue;
        }

        // If the block has already been selected, remove it from the list.
        if (pos == selected[0]) {
          if (pos == "") {   // If we get a mistouch, restart the loop
            if (debug == true) {
              Serial.println("Mistouch"); // confirm we had a mistouch
            }
            continue;
          }

          if (debug == true) {
            Serial.println("Tile deselected: " + selected[0]);  // print the block we selected
          }

          // Draw black so the user knows we've removed it
          draw_at_grid_pos(tft, BOXSIZE, selected[0], ILI9341_BLACK);

          // Draw a grey confirm button in case all were selected
          draw_grey_confirm(tft, BOXSIZE);
          squares_selected--;  // Reduce the counter by 1
          selected[0] = "";  // Remove the entry from our list
          delay(200);
          continue;
        }

        // If squares selected is less then the amount of squares allowed,
        // let the player choose another square.
        if (squares_selected < 1) {
          // If the block state is 0 (undisturbed) or 8 (enemy hidden boat)
          // allow the user to take a shot at that position
          if (game_arr[determine_array_element(pos)].getEnemy() == 0 ||
              game_arr[determine_array_element(pos)].getEnemy() == 8) {
            selected[0] =  pos;  // Store the grid position in our array
            if (debug == true) {
              Serial.println("Tile selected: " + selected[0]);  // print the block we selected
            }

            // Draw green so the user knows we've registered their press
            draw_at_grid_pos(tft, BOXSIZE, pos, ILI9341_GREEN);
            squares_selected++;

            // Restart loop (need to do it this way
            // to allow deselecting when there are 5 tiles)
            if (squares_selected == 1) {
              // Change menu to confirm button once all tiles are selected
              draw_green_confirm(tft, BOXSIZE);
            }
          }
          delay(200);  // 200 ms delay to reduce accidental touches
          continue;
        }

        // If confirm is not selected, restart the loop (wait)
        if (!(get_confirm_or_cancel(point) == 1)) {
          continue;
        }

        // Change menu to confirm button once all tiles are selected
        draw_green_confirm(tft, BOXSIZE);

        if (debug == true){  // debugging last shot
          Serial.println("Last shot is: " + AI_last_shot);
        }

        if (root){
          // Remove the root if a boat is sunk
          // Also reset direction to NULL so we can randomly pick a direction next time
          if (game_arr[determine_array_element(AI_last_shot)].getBlock() == 4){
            root = NULL;
            direction = NULL;

            if (debug == true){
              Serial.println("Boat sunk; root removed.");
            }
          }
        }

        // "Receive" a single block from the AI
        opponent = ai_take_shot(game_arr, AI_last_shot, root, direction);

        // Save our previous shot so we can make an educated shot next time
        AI_last_shot = *opponent;

        // Set to root if there isn't one under certain conditions
        if (not root){

          // Set a root if there is not already a root and we hit a boat
          if (game_arr[determine_array_element(*opponent)].getBlock() == 2){
            root = *opponent;

            if (debug == true){
              Serial.println("Root is: " + root);
            }
          }

          // Set to root if we hit a boat in the past and somehow it was set off of root (edge case)
          for (int i = 0; i < 43; i++){

            // If a root is surrounded by "shot" tiles, skip candidate
            if (game_arr[i].getBlock() == 3){
              if(game_arr[i+1].getBlock() == 0 || game_arr[i+1].getBlock() == 2 ||
                 game_arr[i-1].getBlock() == 0 || game_arr[i-1].getBlock() == 2 ||
                 game_arr[i+7].getBlock() == 0 || game_arr[i+7].getBlock() == 2 ||
                 game_arr[i-7].getBlock() == 0 || game_arr[i-7].getBlock() == 2){

                   root = determine_block(i);

                   if (debug == true){
                     Serial.println("Root is: " + root);
                   }
                   break;
                 }
            }
          }
        }

        if (root){
          // Choose a direction if the last shot was the root
          // or if we have a root and our last shot missed
          // or if we are going to go off of the map with the next shot (based on previous position and direction)
          if ((root == AI_last_shot) || (game_arr[determine_array_element(AI_last_shot)].getBlock() == 0)
              || (AI_last_shot[0] == 'G' && direction == 1) || (AI_last_shot[0] == 'A' && direction == 3)
              || (AI_last_shot[1] == '0' && direction == 2) || (AI_last_shot[1] == '5' && direction == 0)){

            // Continually choose directions until one is chosen that isn't the same as the old one.
            while(true){
              temp_direction = choose_new_direction(root, direction);
              if (debug == true){
                Serial.println("Picking direction");
              }

              if (temp_direction != direction){
                direction = temp_direction;
                break;
              }
            }

            if (debug == true){ // if in debug mode, print the new direction to serial mon
              Serial.print("Direction was reset, new direction is: ");
              Serial.println(String(direction));
            }

            AI_last_shot = root;  // set AI_last_shot to root to "reset" the shooting algorithm
                                  // to reference the next shot from the root
          }
        }

        // Update enemy blocks with what you selected
        game_arr[determine_array_element(selected[
          0])].updateEnemy(update_my_array(game_arr,
            determine_array_element(selected[0])));

        // Update your own block with what your enemy shot
        game_arr[determine_array_element(opponent[
          0])].updateBlock(recieve_turn(game_arr,
            determine_array_element(opponent[0])));

        // Checks if any of my enemy's or my boats are dead
        // if yes, convert the states to display the proper colour
        check_if_enemy_boat_sunk(game_arr);
        check_if_my_boat_sunk(game_arr);

        if (super_debug == true){
          // Print the game states to serial-mon for debugging
          print_blocks_2(game_arr);
          delay(50);
          print_blocks(game_arr);
          delay(50);
        }

        // Check if you have lost or your enemy has lost
        // and set gamestate to 3 if it is
        if (check_deaths(game_arr, squares_allowed, &battleship)) {
          continue;   // If someone died, restart the loop
        }

        // If no one has lost, draw your own map (updated)
        draw_board_self(tft, BOXSIZE, game_arr, opponent, animations);

        // Wait until a touch is registered before continuing
        wait_for_touch(tft, ts, MINPRESSURE);

        // Show your opponents map (updated)
        draw_board_enemy(tft, BOXSIZE, game_arr, &selected[0], animations);

        // Reset our variables to 0 so we can reuse them in the next loop
        squares_selected = 0;  // Reset the squares selected counter to 0
        selected[0] = "";
        opponent[0] = "";  // Remove the entry from the opponent's list
        break;

      case 3:
        // Draws outcome based off of what the alive status is
        draw_outcome(tft, battleship.get_is_alive());
        // Wait until a touch is registered before resetting the game
        wait_for_touch(tft, ts, MINPRESSURE);
        resetFunc();  // Resets the arduino
        break;
    }
  }
}

int main() {
  setup_arduino();
  // Main menu phase (to determine game mode)
  main_menu(tft, BOXSIZE);

  // Play a 2 player game
  if (battleship.get_game_mode() == 2){
    play_game();
  }

  // Play a 1 player game
  else if (battleship.get_game_mode() == 1){
    play_game_solo();
  }
}
