#include "assets.h"

Player::Player(int16_t start_x, int16_t start_y){
    x = start_x;
    y = start_y;
    xs = 0; ys = 0;
    direction = 1;
    on_floor = 1;
    frame_time = 0;
}



void Player::update(float delta_time){
    x += xs * delta_time;
    y += ys * delta_time;
    if(y >= 90){
        y = 90;
        ys = 0;
        on_floor = 1;
    }
    if(!on_floor) ys += fg * delta_time;
    
    //animation
    if(frame_time >= 0.1f){
        if(xs > 0) direction = 1;
        else if(xs != 0) direction = 0;

        //idle
        if(on_floor && xs == 0){
            frame = {0, 0};
        }
        //walk animation
        else if(on_floor){
            frame.first = 0;
            frame.second++;
            if(frame.second > sprite_player_info[frame.first][1]) frame.second = 0;
        }
        //jump
        else if(!on_floor) frame = {1, 0};

        frame_time -= 0.1f;
    }
}

void Player::right(float delta_time){
    if(xs < 40)xs += 100 * delta_time;
}

void Player::left(float delta_time){
    if(xs > -40)xs -= 100 * delta_time;
}

void Player::jump(){
    if(on_floor){
        ys = -80;
        on_floor = 0;
    }
}

void Player::stop(float delta_time){
    if(xs > 0.4)xs -= delta_time * 80;
    else if(xs < -0.4) xs += delta_time * 80;
    else xs = 0;
}