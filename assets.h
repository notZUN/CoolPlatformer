#ifndef ASSETS_H
#define ASSETS_H
#include <cstdint>
#include <utility>

extern const uint8_t fg;

//sprites
extern const uint8_t sprite_player[4][36];
extern const uint8_t sprite_player_info[2][2];
extern const uint16_t pallete[256];


//functions and variables
class Player{
public:
    float x, y, xs, ys, frame_time;
    bool direction, on_floor;
    std::pair<uint8_t, uint8_t> frame;

    Player(int16_t start_x, int16_t start_y);
    void jump();
    void right(float delta_time);
    void left(float delta_time);
    void update(float delta_time);
    void stop(float delta_time);

};
#endif
