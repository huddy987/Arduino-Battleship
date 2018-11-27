#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen
#include "touch_handler.h"  // touch handler header file
#include "draw_handler.h" // draw handler header file
#include "game.h" // game class
#include "client.h" // client class
#include "player.h" // Player class
//#include "data_handler.h"


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

// Game global object with initial state set to 0, game_mode set to 0 (since we haven't selected a mode yet)
Game battleship = Game(0, 0);

// Client global object
Client client;

// Define grid box size (40x40)
int BOXSIZE = 40;

// Define how many squares are allowed
int squares_allowed = 5;

void setup() {
  init();
  Serial.begin(9600);
  Serial3.begin(9600);
  Serial.println("Welcome to Battleship!");

  tft.begin();
  // Draw menu
  draw_menu(tft);
}

// Handles main menu functionality
void main_menu(Adafruit_ILI9341 tft, TSPoint point, int BOXSIZE){
  // Check what mode we are entering and update the battleship's game mode
  battleship.update_game_mode(tft, point);

  // Send "I am ready!" message to other arduino
  client.send_ready_message();

  // Wait for opponenet to respond
  client.wait(tft);

  // If we make it down here, it means a screen press has been registered on both arduinos
  // so we should update the game state
  battleship.update_state(1);

  // Draw an empty map
  draw_empty_map(tft, BOXSIZE);
}

// Handles game set up
void setup_game(Adafruit_ILI9341 tft, TSPoint point, int BOXSIZE){

}

void play_game(){
  // Calibrate minimum pressure to be considered a touch
  #define MINPRESSURE 10

  // Variables for play and setup phase
  int squares_selected = 0;   // Tracks how many squares are selected in both phases cases 1 and 2
  String selected[squares_allowed] = {};  // Array containing the blocks selected (in "A0" notation)
  int already_selected;   // Flag that checks if a tile has already been selected
  String *opponent;   // Opponent block array

  while(1){

    // Get a point and map it to the screen dimensions
    TSPoint point = get_point(tft, ts);

    // If the point doesn't have enough pressure, restart from the top (no press registers)
    if (point.z < MINPRESSURE) {
     continue;
    }

    // Stores grid position in "A0" notation
    String pos = get_grid_position(point, BOXSIZE);

    switch (battleship.get_state()) {

      case 0:
        main_menu(tft, point, BOXSIZE);
        break;

      case 1:
        already_selected = 0;

        // If cancel is pressed, reset everything
        if(get_confirm_or_cancel(point) == 2){  // If cancel is pressed
          squares_selected = 0; // Reset the squares selected counter to 0
          clear_all_selections(tft, BOXSIZE, selected, squares_allowed);  // Draws board in a reset state
          for(int i = 0; i < squares_allowed; i++){ // Replace selected squares in player's own array with 0
            selected[i] = "";
          }
          continue; // Restart the loop
        }
        // If confirm is pressed before all tiles are selected, ignore the press
        if(get_confirm_or_cancel(point) == 1 and squares_selected < 5){
          continue;
        }

        // If a block has already been selected, remove it from the list
        for(int i = 0; i < squares_allowed; i++){
          // If the block has already been selected, remove it from the list.
          if(pos == selected[i]){
            Serial.println(selected[i]);
            draw_at_grid_pos(tft, BOXSIZE, selected[i], ILI9341_BLACK); // Draw black so the user knows we've removed it
            draw_grey_confirm(tft, BOXSIZE);  // Draw a grey confirm button in case all were selected
            squares_selected--; // Reduce the counter by 1
            selected[i] = "";  // Remove the entry from our list
            already_selected = 1;
          }
        }
        // If flag is 1, restart the loop (delay to reduce accidental touches)
        if (already_selected == 1){
          delay(200);
          continue;
        }

        // If squares selected is less then the amount of squares allowed, let the player choose another square.
        if(squares_selected < squares_allowed){
          selected[squares_selected] =  pos;  // Store the grid position in our array
          Serial.println(selected[squares_selected]);
          draw_at_grid_pos(tft, BOXSIZE, pos, ILI9341_GREEN); // Draw green so the user knows we've registered their press
          squares_selected++;
          //Serial.println(squares_selected);

          // Restart loop (need to do it this way to allow deselecting when there are 5 tiles)
          if (squares_selected == squares_allowed){
            // Change menu to confirm button once all tiles are selected
            draw_green_confirm(tft, BOXSIZE);
          }
          delay(200);   //200 ms delay to reduce accidental touches
          continue;
        }

        // If confirm is not selected, restart the loop (wait)
        if(!(get_confirm_or_cancel(point) == 1)){
          continue;
        }
        // Send "I am ready!" message to other arduino
        client.send_ready_message();

        // Wait for opponenet to respond
        client.wait(tft);

        // Send own selected tiles to opponent
        client.send_ships(selected, squares_allowed);

        delay(200);  // Small delay so that all the serial data can be sent

        // Receive opponent selected tiles
        opponent = client.receive_ships(squares_allowed);

        // Update player class to include opponent array TODO

        // If we make it down here, it means both arduinos have selected their tiles
        // So we should update the gamestate
        battleship.update_state(2);

        // Draw an empty map
        draw_empty_map(tft, BOXSIZE);

        // Reset already_selected to 0 so we can use it in the next phase
        already_selected = 0;
        squares_selected = 0; // Reset the squares selected counter to 0
        for(int i = 0; i < squares_allowed; i++){ // Replace selected squares in player's own array with 0
          selected[i] = "";
        }
        break;

      case 2:
        // Show green confirm button once a grid pos is pressed
        already_selected = 0;

        // If cancel is pressed, reset everything
        if(get_confirm_or_cancel(point) == 2){  // If cancel is pressed
          squares_selected = 0; // Reset the squares selected counter to 0
          clear_all_selections(tft, BOXSIZE, selected, squares_allowed);  // Draws board in a reset state
          for(int i = 0; i < squares_allowed; i++){ // Replace selected squares in player's own array with 0
            selected[i] = "";
          }
          continue; // Restart the loop
        }
        // If confirm is pressed before all tiles are selected, ignore the press
        if(get_confirm_or_cancel(point) == 1 and squares_selected < 1){
          continue;
        }
        // If the block has already been selected, remove it from the list.
        if(pos == selected[0]){
          Serial.println(selected[0]);
          draw_at_grid_pos(tft, BOXSIZE, selected[0], ILI9341_BLACK); // Draw black so the user knows we've removed it
          draw_grey_confirm(tft, BOXSIZE);  // Draw a grey confirm button in case all were selected
          squares_selected--; // Reduce the counter by 1
          selected[0] = "";  // Remove the entry from our list
          already_selected = 1;
        }
        // If flag is 1, restart the loop (delay to reduce accidental touches)
        if (already_selected == 1){
          delay(200);
          continue;
        }

        // If squares selected is less then the amount of squares allowed, let the player choose another square.
        if(squares_selected < 1){
          selected[0] =  pos;  // Store the grid position in our array
          Serial.println(selected[0]);
          draw_at_grid_pos(tft, BOXSIZE, pos, ILI9341_GREEN); // Draw green so the user knows we've registered their press
          squares_selected++;
          //Serial.println(squares_selected);

          // Restart loop (need to do it this way to allow deselecting when there are 5 tiles)
          if (squares_selected == 1){
            // Change menu to confirm button once all tiles are selected
            draw_green_confirm(tft, BOXSIZE);
          }
          delay(200);   //200 ms delay to reduce accidental touches
          continue;
        }

        // If confirm is not selected, restart the loop (wait)
        if(!(get_confirm_or_cancel(point) == 1)){
          continue;
        }

        // Change menu to confirm button once all tiles are selected
        draw_green_confirm(tft, BOXSIZE);

        // Wait for other arduino

        // Update enemy map from your selection, and your own map from data received over serial3

        // Check if you have lost

        // Wait for your opponent to tell you if they lost

        // If your opponent lost, set gamestate to 3, game is over

        // Else draw your own map (updated) for 5 seconds

        // Show your opponents map (updated)
        break;

      case 3:
        // Draws outcome based off of what the alive status is
        draw_outcome(tft, battleship.get_is_alive());
        break;
    }
  }
}

int main(){
  setup();
  play_game();
}
