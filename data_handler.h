#ifndef _DATA_HANDLER_H_
#define _DATA_HANDLER_H_


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

uint8_t determine_array_element(String grid_pos);

bool check_all_boat_sunk(Block play_arr[], uint8_t boat_id);

uint8_t kill_entire_boat(Block play_arr[], uint8_t boat_id);

void send_boat_death(Block play_arr[], uint8_t boat_id, uint8_t boat_death);

void recieve_turn(Block player_array[], uint8_t enemy_block_number);

void update_my_array(Block play_arr[], uint8_t my_block_number);

void making_a_turn(String grid_pos, Block player_array[]);


#endif
