#ifndef _BLOCK_H_
#define _BLOCK_H_

class Block{
  private:
    uint8_t boat_id;
    uint8_t block_state;
    uint8_t enemy_state;
  public:
    Block();
    void updateBoat(uint8_t num);
    void updateBlock(uint8_t num);
    void updateEnemy(uint8_t num);
    uint8_t getBoat();
    uint8_t getBlock();
    uint8_t getEnemy();
};

#endif
