#include "assets.h"
#include<iostream>
Player::Player(int16_t start_x, int16_t start_y){
    x = start_x;
    y = start_y;
    xs = 0; ys = 0;
    direction = 1;
    on_floor = 1;
    frame_time = 0;
}

Object::Object(uint8_t typ, float start_x, float start_y, float start_xs, float start_ys){
  x = start_x;
  y = start_y;
  xs = start_xs;
  ys = start_ys;
  type = typ;

}

void Player::update(float delta_time){
      //physic
    //collisions
      //search nearest block
    float dist_x = 99999999.0f;
    float dist_y = 99999999.0f;
    if(!on_floor) ys += fg * delta_time;
    for(auto &p: blocks){
      switch(p->type){
      case 1:
        //along y-axis 
        if(p->x < x + 7 + xs * delta_time && x < p->x + 8 + xs * delta_time){
          if(abs(int((window_height - p->y) - y)) < dist_y){
            dist_y = abs(int((window_height - p->y) - y));
            near_block_y = p;
          }
        }
        //along x-axis 
        if(window_height - p->y < y + ys * delta_time + 6 && y + ys * delta_time < window_height - p->y && p->x < window_width + x){
          if(abs(int(p->x - x)) < dist_x){
            dist_x = abs(int(p->x - x));
            near_block_x = p;
          }
        }
      break;
      case 2:
        //money
        if((p->y - y) * (p->y - y) + (p->x - x) * (p->x - x) < 50){
          p->ys-=20;
          money_collected++;
        }
      break;
      }
    }
    //will player collide?
      //along y-axis
    if(dist_y > 1024){
      near_block_y = nullptr;
      on_floor = false;
    }
    else if(y > window_height - near_block_y->y && y + ys * delta_time < window_height - near_block_y->y + 1){
      ys *= -0.4;
    }
    else if(y + 7 < window_height - near_block_y->y && y + 7 + ys * delta_time > window_height - near_block_y->y){
      ys = 0;
      on_floor = true;
    }
      //along x-axis 
      if(dist_x > 1024) near_block_x = nullptr;
      else if(near_block_x->x < x + 7 + xs * delta_time && x < near_block_x->x + 9 + xs * delta_time){
        xs = 0;
      }

    

    //change in position
    y += ys * delta_time;
    x += xs * delta_time;
    if((x + xs * delta_time) - cam_x < 5){
      xs *= -1.2;
    }
    std::cout << x << '\n';
    if(y >= window_height - 12){
        y = window_height - 12;
        ys = 0;
        on_floor = 1;
    }
    
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

void create_block(uint8_t typ, int x, int y){
  unsigned int free = free_blocks();
  blocks[free]->type = typ;
  blocks[free]->x = x;
  blocks[free]->y = y;
  blocks[free]->xs = 0;
  blocks[free]->ys = 0;

}

void Object::update(float delta_time){
  switch(type){
    case 2:
      if(ys < 0){
        ys += 500 * delta_time;
        if(ys > -5)type = 0;
        y += ys * delta_time;
      }
    break;
  }
}

unsigned short free_blocks(){
  for(unsigned short i = 0; i < 1000; i++){
    if(blocks[i] -> type == 0) return i;
  }
  return 1000;
}

uint16_t abs(short num){
  if(num < 0) return num * -1;
  else return num;
}
