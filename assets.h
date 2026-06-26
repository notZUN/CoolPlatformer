#ifndef ASSETS_H
#define ASSETS_H
#include <cstdint>
#include <utility>

#include <SDL2/SDL_mixer.h>

extern float cam_x;
extern const uint8_t window_height, window_width;
extern const uint8_t fg;
extern int unsigned generation_x;
extern int money_collected;
//sprites
extern const uint8_t sprite_player[4][36];
extern const uint8_t sprite_player_info[2][2];
extern const uint16_t pallete[256];
extern const uint8_t money[25];
extern const bool numbers[10][15];
extern const uint8_t spike[15];
extern bool running;

extern Mix_Chunk* sound_jump;
extern Mix_Chunk* sound_money;

//functions and variables
class Object{
public:
    float x, y, xs, ys;
    uint8_t type;

    Object(uint8_t typ, float start_x, float start_y, float start_xs, float start_ys);
    void update(float delta_time);
};
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
    //Object* near_block(bool direc);
};


extern Object* near_block_x;
extern Object* near_block_y;
uint16_t abs(short num);
void create_block(uint8_t typ, int x, int y);
extern Object* blocks[1000];
unsigned short free_blocks();

#endif
