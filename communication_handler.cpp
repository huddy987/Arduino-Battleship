#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
// include the data_handler here for updating arrays

//NOTE: remember to initialize Serial3 in the setup similar to diffie

/* TODO:
  1. P1 will send the block selected
  2. P2 will recieve the block and update their game array
  3. send to P1 if hit or miss
    a. special case if all points of the ship has been hit
  4. P1 will update their game state accordingly.
  5. P2's turn.


}
void send_the_block(String grid_pos){
  Serial3.write(grid_pos);

}

/* each arduino will have an array from A5 to G0
  >>> element 0 is block A5 and element 41 is block G0
    to keep track of which blocks have been selected
    keep this array in the stack of the main file in order
    make it permanent
*/
void recieve_the_block(){
  String enemy_block = Serial3.read();

}
