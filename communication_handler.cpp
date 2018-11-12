#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
// include the data_handler here for updating arrays

//NOTE: remember to initialize Serial3 in the setup similar to diffie

/* TODO:
  0. each arduino will have an array from A5 to G0
    >>> element 0 is block A5, el. 1 i B5... element 41 is block G0
      to keep track of which blocks have been selected
      keep this array in the stack of the main file in order
      make it permanent
  1. make both arduinos store the blocks entered in both arduinos
    a. this will be done in the form of an array.
    b. a block can have 6 states.
      0 = undisturbed
      1 = has a boat hidden; not shot
      2 = has a boat that's been shot
      3 = has been shot but no boat
      4 = enemy's: shot but no boat
      5 = enemy's: shot but yes boat
    c. the array element will store a number
      corresponding to the state of a block
*/


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
