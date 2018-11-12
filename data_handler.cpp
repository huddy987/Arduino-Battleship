#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
///

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

return uint16_t determine_array_element(String grid_pos){
  //idk if I can index strings so this should be fun
  // maybe just a bunch of switch cases
  // super easy switch case. just hard code everything
  }
