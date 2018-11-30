#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen
#include "block.h"
#include "client.h"
#include "draw_handler.h"

// Waits for the other client to respond when data must be sent simultaneously
void Client::wait(Adafruit_ILI9341 tft){
  // If we are waiting, draw a waiting screen
  if (!Serial3.available()){
    // Draw a waiting screen while waiting for the other arduino to respond
    draw_waiting(tft);
  }

  // If we are waiting, loop until the other arduino responds
  while(!Serial3.available()){
    continue;
  }
  // Read the buffer to clear it if we recieve something
  while(Serial3.available() > 0){
    Serial3.read();
  }
}

// Prints a "ready" character over serial3
void Client::send_ready_message(){
  // Write a single Z to our opponent to signal that we are ready
  Serial3.write("Z");   // Z was chosen as not to conflict with grid positions
}

// Sends own grid coordinates over serial3
void Client::send_ships(String boat_array[], int length){
  for(int i = 0; i < length; i++){
    Serial3.print(char(boat_array[i][0]));
    Serial3.print(char(boat_array[i][1]));
  }
}

// Receive opponents grid coordinates over serial3
// https://stackoverflow.com/questions/12992925/c-correct-way-to-return-pointer-to-array-from-function
String *Client::receive_ships(int length){
  String *opponent_ships = new String[length];  // New ensures that the memory address is never overwritten
  int counter = 0;
  int current_index = 0;
  String tile;
  while(true){  // Constantly read bytes
    if(current_index == (length)){  // If our counter is equal to the desired length, break
      break;
    }
    else{
      char byte = Serial3.read();
      if(counter < 2){
        tile = String(tile + byte); // Append the read byte to the tile
        counter ++;
      }
      if(counter == 2){  // If we have read 2 characters
        opponent_ships[current_index] = tile;
        Serial.println(opponent_ships[current_index]);
        tile = "";  // Reset the string
        current_index++;  // Increment the counter
        counter = 0;
      }
    }
  }
  return opponent_ships;
}
