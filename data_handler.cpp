#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

// (*(play_arr + #)) --> this is the block object
/*
               A   B    C     D    E   F    G
      -----------------------------------------
      |  M  | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 5
      |     |----------------------------------|
      |  E  | 28 | 29 | 30 | 32 | 32 | 33 | 34 | 4
      |     |----------------------------------|
      |  N  | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 3
      |     |----------------------------------|
      |  U  | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 2
      |     |----------------------------------|
      |  "  | 07 | 08 | 09 | 10 | 11 | 12 | 13 | 1
      |     |----------------------------------|
      |  "  | 00 | 01 | 02 | 03 | 04 | 05 | 06 | 0
      -----------------------------------------
*/


/*
const int gen_pin = 1;

void setup() {
    init();
    pinMode(gen_pin, INPUT);
    Serial.begin(9600);
    Serial3.begin(9600);
    Serial.flush();
    Serial3.flush();
}
*/


/* TODO:
0. each arduino will have an array from A5 to G0
  >>> element 0 is block A0, element 1 is B0... element 41 is block G5
    to keep track of which blocks have been selected
    keep this array in the stack of the main file in order
    make it permanent
1. make both arduinos store the blocks entered in both arduinos
  a. this will be done in the form of a 2D array
    - array of 42 elements (representing the blocks)
    - each of the 42 elements will contatain 2 integers:
      > block state
      > boat id
  b. a block can have 8 states.
    0 = undisturbed
    1 = has been shot but no boat
    2 = has a boat hidden; not shot
    3 = has a boat that's been shot
    4 = has a full boat sunk
    5 = enemy's: shot but no boat
    6 = enemy's: shot but yes boat
    7 = enemy's: full boat sunk
  c. the array element will store a pointer to act as tuple.
    i. the first element of the tuple represents the ship id.
    ii. second element stores the block state
    iii. if all blocks of a ship has been hit, it will be sunk
*/


/*
  uint8_t boat_id - the id of the boat that resides there
  uint8_t block_state - the state of my block
  uint8_t enemy_state - the enemy block state from my POV

*/
class Block {
    public:
      uint8_t boat_id, block_state,enemy_state;
      Block()  // declares the constructor --> initiates all values as 0
      {
        boat_id = 0;
        block_state = 0;
        enemy_state = 0;
      }
      uint8_t getBoat(){return boat_id;}
      uint8_t getBlock(){return block_state;}
      uint8_t getEnemy(){return enemy_state;}
};


/*
  when in doubt, double check.

  Inputs: grid_pos - A0 to %5
  Output: element - which block number it is
*/
uint8_t determine_array_element(String grid_pos){
  //idk if I can index strings in C++ so this should be fun to test
  uint8_t column = static_cast<int>(grid_pos[0]) - 64;
  uint8_t row = static_cast<int>(grid_pos[1]) - 48;
  uint8_t element = column + (row * 7) -1;
  return element;
}


/*   DEBUGGING
// printing this for debugging
void print_blocks(Block player_array[]){
  Serial.println("My Block States");
  for(int i=1; i<43; i++){
    if ((i%7)==0) {Serial.print((*(player_array + i - 1)).getBlock());Serial.println();}
    else {Serial.print((*(player_array + i - 1)).getBlock());}
  }
  Serial.println();
}

// printing for debugging
void print_boats(Block player_array[]){
  Serial.println("My Enemy States");
  for(int i=1; i<43; i++){
    if ((i%7)==0) {Serial.print((*(player_array + i - 1)).getEnemy());Serial.println();}
    else {Serial.print((*(player_array + i - 1)).getEnemy());}
  }
  Serial.println();
}
*/



/*
Inputs: the player's array and the boat id
Output: true - if all blocks with the same boat id has been shot (==3)
        false - otherwise
*/
bool check_all_boat_sunk(Block play_arr[], uint8_t boat_id){
  uint8_t i, return_val;
  uint8_t boat_count = 0;
  uint8_t shot_count = 0;

  for(i=0;i<42;i++) {   /// count how many blocks has the same boat id
    if (((*(play_arr + i)).getBoat()) == boat_id) {
      boat_count++;
    
    // if it's been shot and same boat id
    if (((*(play_arr + i)).getBlock()) == 3) {
      shot_count++;
    }
  }
  if (boat_count == shot_count){
    return_val = true;
  } else {return_val = false;}
  }
  return return_val;
}


/*
This will transform all blocks with the same boat id into shot (==4)
Inputs: the player's array and the boat id
Output: the number of blocks that's been equated to 4 (died)
 */
uint8_t kill_entire_boat(Block play_arr[], uint8_t boat_id){
  uint8_t i;
  uint8_t count = 0;
  for(i=0;i<42;i++) {   /// TODO: check here later
  if (((*(play_arr + i)).getBoat()) == boat_id) {
    (*(play_arr + i)).block_state = 4;
    count++;
  }
 }
 return count;
}

/*
This should send '7' followed by the amount of blocks that died.
then the block numbers of the blocks

Inputs: the player's array
        boat id  - id of the boat that died
        boat_death - how many blocks died
Output: none. for communication only

*/
void send_boat_death(Block play_arr[], uint8_t boat_id, uint8_t boat_death){
  // Serial3.write(7);
  // Serial3.write(boat_death);

  // Serial.print("Dying boats: "); Serial.println(boat_death);  // --> this is for debugging
  for(int i=0;i<42;i++) {
  if (((*(play_arr + i)).getBoat()) == boat_id) {
    if ((*(play_arr + i)).block_state == 4) {  // this embedded if statement should always be true
      // Serial3.print(i)  --> send the blocks that died

      // Serial.println(i);
    }
  }
 } //Serial.println();
}


/*
will check what the state of my block is
if == 0, send them a 5. This means they shot nothing
if == 2, check if they sunk an entire boat.
  if they sunk an entire boat. send_boat_death(...); executes
  else, send them a 6

Inputs: the player's array
        enemy_block_number - the block that the enemy wants to attack
Outputs: none.
*/
void recieve_turn(Block player_array[], uint8_t enemy_block_number){
uint8_t boat_id, boat_death;
  switch ((*(player_array + enemy_block_number)).getBlock()) {
    case 0 :  // undisturbed --> make it a 1 on my block state
      (*(player_array + enemy_block_number)).block_state = 1;
      // Serial3.write(5); // tell them that shot but no boat;
      break;

    case 2 :  // boat hidden; not shot
      (*(player_array + enemy_block_number)).block_state = 3;
      boat_id = (*(player_array + enemy_block_number)).getBoat();

      //if all blocks with the same boat has been shot, the if-block will execute
      if (check_all_boat_sunk(player_array, boat_id)){
        boat_death = kill_entire_boat(player_array, boat_id);
        send_boat_death(player_array, boat_id, boat_death);

      } else {Serial3.write(6);}
      break;
    default :
      Serial.print("Error");
    }
}



/*
this is what happens after I shoot my enemy's blocks
Enemy will tell me what happened and I will update 
my array accordingly

Switch Case
  -5: I shot something but it didn't sink
  -6: I shot a boat but only one block of it, not the whole thing
  -7: I killed the entire boat.
      --> this should then recieve the number of blocks that died
      --> then, in a for-loop, recieve the blocks make them = 7


Inputs: the player's array
        my_block_number - the block number that I shot
Outputs: none.
*/
void update_my_array(Block play_arr[], uint8_t my_block_number){
  uint8_t boat_id;
  uint8_t block_died;
  uint8_t boat_deaths;
  uint8_t enemy_response;

  // read the enemy's response here
  // enemy_response = Serial3.read();

  switch (enemy_response) {
    case 5:
      // Serial.println("5"); --> for debugging only
      (*(play_arr + my_block_number)).enemy_state = 5;
      break;

    case 6 :
      // Serial.println("6"); --> for debugging only
      (*(play_arr + my_block_number)).enemy_state = 6;
      break;

    case 7 :
      // Serial.println("7");  --> for debugging only

      // boat_deaths = Serial3.read();  // --> how many blocks died
      // for (int i=0;i<boat_deaths;i++){
        // block_died =  Serial.read();
        // (*(play_arr + block_died)).enemy_state = 7;
      break;
    default :  // should never execute
      Serial.println("Error in update_my_array");
  }
}




/* 
  This function might be extraneous but the logic is here

  1. send them which block you want to attack
  2. recieve which block they want to attack
  3. recieve_turn - see how their attack affects me
  4. update_my_array - see how my attack affected them
*/
void making_a_turn(String grid_pos, Block player_array[]) {
  uint8_t my_block_number = determine_array_element(grid_pos);
  Serial3.write(my_block_number);

  uint8_t enemy_block_number = Serial3.read();
  recieve_turn(player_array, enemy_block_number);
  update_my_array(player_array, my_block_number);
}




// Initialize the player's array by this.
// all 3 values of all blocks will == 0 initially
/*
Block game_arr[] = {Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),
    Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),
    Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),
    Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block()};
*/



/*  DEBUGGING 
  This main is for debugging purposes only
  Everything under this is not needed for your purpose

int main() {
  setup();

  /// I could also implement a default constructor but for now,
  /// they all start with a value of 0
  // create an array of blocks will all have values of zero
  Block game_arr[] = {Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),
    Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),
    Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),
    Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block(),Block()};
    // --> idk how to put this in a for loop


    (game_arr[15]).block_state = 2;
    (game_arr[16]).block_state = 3;
    (game_arr[17]).block_state = 3;
    (game_arr[41]).block_state = 2;
    (game_arr[15]).boat_id = 90;
    (game_arr[16]).boat_id = 90;
    (game_arr[17]).boat_id = 90;
    (game_arr[41]).boat_id = 90;


    print_blocks(game_arr);
    print_boats(game_arr);
    recieve_turn(game_arr, 15);
    print_blocks(game_arr);
    print_boats(game_arr);
    recieve_turn(game_arr, 41);
    print_blocks(game_arr);
    print_boats(game_arr);
    update_my_array(game_arr, 8);
    print_blocks(game_arr);
    print_boats(game_arr);
    // Serial.println((game_arr[9]).getBoat);

    while (true) { }
        // if there are bytes in the user's arduino buffer
    Serial.end();
    return 0;
} */