#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen
#include "block.h"
#include "client.h"
#include "draw_handler.h"

Block::Block(){
      boat_id = 0;
      block_state = 0;
      enemy_state = 0;
}

void Block::updateBoat(uint8_t num){boat_id = num;}
void Block::updateBlock(uint8_t num){block_state = num;}
void Block::updateEnemy(uint8_t num){enemy_state = num;}
uint8_t Block::getBoat(){return boat_id;}
uint8_t Block::getBlock(){return block_state;}
uint8_t Block::getEnemy(){return enemy_state;}
