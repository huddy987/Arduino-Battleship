#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen
#include "block.h"
#include "client.h"
#include "draw_handler.h"

// Block object constructor
Block::Block(){
      boat_id = 0;
      block_state = 0;
      enemy_state = 0;
      enemy_boat_id = 0;
}

// updates the boat_id
void Block::updateBoat(uint8_t num) {
    boat_id = num;
}

// updates the block_state
void Block::updateBlock(uint8_t num) {
    block_state = num;
}

// updates enemy_state
void Block::updateEnemy(uint8_t num) {
    enemy_state = num;
}

// updates enemy_boat_id
void Block::updateEnemyBoat(uint8_t num) {
    enemy_boat_id = num;
}

// returns boat_id
uint8_t Block::getBoat(){
    return boat_id;
}

// returns block_state
uint8_t Block::getBlock() {
    return block_state;
}

// returns enemy_state
uint8_t Block::getEnemy() {
    return enemy_state;
}

// returns enemy_boat_id
uint8_t Block::getEnemyBoat() {
    return enemy_boat_id;
}