#ifndef _BLOCK_H_
#define _BLOCK_H_
/*
Block States
    0 = undisturbed

    User's
    1 = has been shot but no boat
    2 = has a boat hidden; not shot
    3 = has a boat that's been shot
    4 = has a full boat sunk

    Enemy's
    5 = enemy's: shot but no boat
    6 = enemy's: shot but yes boat
    7 = enemy's: full boat sunk
    8 = enemy's: has boat but not shot

Boat ID's are just 1,2,3
*/

class Block{
  private:
    uint8_t boat_id;
    uint8_t block_state;
    uint8_t enemy_state;
    uint8_t enemy_boat_id;
  public:
    // Constructor
    Block();

    // updates the boat_id
    void updateBoat(uint8_t num);

    // updates the block_state
    void updateBlock(uint8_t num);

    // updates enemy_state
    void updateEnemy(uint8_t num);

    // updates enemy_boat_id
    void updateEnemyBoat(uint8_t num);

    // returns boat_id
    uint8_t getBoat();

    // returns block_state
    uint8_t getBlock();
 
    // returns enemy_state
    uint8_t getEnemy();
 
    // returns enemy_boat_id
    uint8_t getEnemyBoat();
};

#endif
