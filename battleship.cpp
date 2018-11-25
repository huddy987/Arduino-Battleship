#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen
#include "touch_handler.h"  // touch handler header file
#include "draw_handler.h" // draw handler header file
#include "game.h" // game class
#include "client.h" // client class
#include "player.h" // Player class


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
#define BOXSIZE 40

// Define how many squares are allowed
#define squares_allowed 5

void setup() {
  init();
  Serial.begin(9600);
  Serial3.begin(9600);
  Serial.println("Welcome to Battleship!");

  tft.begin();
  // Draw menu
  draw_menu(tft);
}
void play_game(){
  // Calibrate minimum pressure to be considered a touch
  #define MINPRESSURE 10

  // These variables need to be moved into a function TODO
  int squares_selected = 0;
  String selected[squares_allowed] = {};
  String pos;
  int already_selected;
  String *opponent;

  while(1){

    // Get a point and map it to the screen dimensions
    TSPoint point = get_point(tft, ts);

    // If the point doesn't have enough pressure, restart from the top (no press registers)
    if (point.z < MINPRESSURE) {
     continue;
    }

    switch (battleship.get_state()) {

      case 0:
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
        break;

      case 1:
        already_selected = 0;
        // Stores grid position in "A0" notation
        pos = get_grid_position(point, BOXSIZE);

        // If cancel is pressed, reset everything
        if(get_confirm_or_cancel(point) == 2){  // If cancel is pressed
          squares_selected = 0; // Reset the squares selected counter to 0
          clear_all_selections(tft, BOXSIZE, selected, squares_allowed);  // Draws board in a reset state
          for(int i = 0; i < squares_allowed; i++){ // Replace selected squares in player's own array with 0
            selected[i] = "";
          }
          continue; // Restart the loop
        }

        // If squares selected is less then the amount of squares allowed, let the player choose another square.
        if(squares_selected < squares_allowed){
          for(int i = 0; i < squares_allowed; i++){
            // If the block has already been selected, remove it from the list.
            if(pos == selected[i]){
              Serial.println(selected[i]);
              draw_at_grid_pos(tft, BOXSIZE, selected[i], ILI9341_BLACK); // Draw black so the user knows we've removed it
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
          else{
            selected[squares_selected] =  pos;  // Store the grid position in our array
            Serial.println(selected[squares_selected]);
            if(selected[squares_selected]){   // Double check in case we accidentally press on a white border
              draw_at_grid_pos(tft, BOXSIZE, pos, ILI9341_GREEN); // Draw green so the user knows we've registered their press
              squares_selected++;
              Serial.println(squares_selected);
            }
          }

          // If we still have less squares then the allowed amount, restart the loop.
          if(squares_selected < squares_allowed){
            delay(200);   //200 ms delay to reduce accidental touches
            continue;
          }
        }

        // Change menu to confirm button once all tiles are selected
        draw_green_confirm(tft, BOXSIZE);

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

        delay(50);  // Small delay so that all the serial data can be sent TODO test on richmond laptop

        // Receive opponent selected tiles
        opponent = client.receive_ships(squares_allowed);

        // Update player class to include opponent array TODO
        // Something like: Player player = Player(18, selected, opponent)

        // If we make it down here, it means both arduinos have selected their tiles
        // It is time to update the gamestate.
        battleship.update_state(2);

        // Draw an empty map
        draw_empty_map(tft, BOXSIZE);
        break;

      case 2:
        // Wait for player to lock in response
        break;

      case 3:
        // Wait for opponent to lock in response
        break;

      case 4:
        // Show final game state
        break;
    }
  }
}

int main(){
  setup();
  play_game();
}
