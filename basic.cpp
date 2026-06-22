#include <SDL2/SDL_timer.h>
#include <cstdint>
#include <SDL2/SDL.h>
#include <ctime>
#include "assets.h"
#include <iostream>

//create base variables for window width and hight, arrays for my "painting"(I don't know how call it), etc.
float delta = 0.016f;
const uint8_t fg = 100; //gravity
const uint8_t window_width = 170, window_height = 96;
uint8_t camera[window_width * window_height];
uint16_t final_screen[window_width * window_height];
float cam_x = 0, cam_y = 0;
unsigned int generation_x = 20;
uint8_t chance_money = 28;
float chance_spike = 25;
int spike_quantity_max = 2;
float anim_money_y = 0;
bool anim_money_direction = 0;
int money_collected = 0;
Object* blocks[1000];

Object* near_block_x = nullptr;
Object* near_block_y = nullptr;


int main(){
    srand(time(NULL));
    const uint8_t* keystate = SDL_GetKeyboardState(NULL);

    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Renderer* render = nullptr;

    //create window
    SDL_Window* window = SDL_CreateWindow(
        "Project_01",
        100, 
        100, 
        window_width * 5, 
        window_height * 5,
        SDL_WINDOW_SHOWN
    );

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //create texture
    SDL_Texture* texture = SDL_CreateTexture(
    render,
    SDL_PIXELFORMAT_RGB565,
    SDL_TEXTUREACCESS_STREAMING,
    window_width, window_height
);

    SDL_Event event;
    
    bool running = 1;
    uint32_t tick_start;

    //cleaning array of camera
    for(uint16_t i = 0; i < window_height; i++){
            for(uint16_t j = 0; j < window_width; j++){
            camera[i*window_width+j] = 0;
        }
    }

    Player player(20, 90);

for(int i = 0; i < 1000; i++){
  blocks[i] = new Object(0, 0, 0, 0, 0);
}
    
//game
    while(running){
        tick_start = SDL_GetTicks();
        SDL_RenderClear(render);
        
        //delete behind window
        for(auto &p: blocks){
          if(p->x < cam_x)p->type = 0;
        }

        //generation
        while(cam_x + 200 > generation_x){
            uint8_t rand_num = (rand() & 7) + 1, rand_y = (abs(rand()) & 7) * 4 + 16;
            for(int i = 0; i < rand_num; i++)create_block(1,generation_x + (i * 8), rand_y);
            
            //on blocks 
            //money 
            if((rand() & 31) > chance_money){
              create_block(2, generation_x + 1 + (rand() % rand_num) * 8, (window_height - rand_y) - 8);
            }

            //spikes 
            else if((rand() & 31) > chance_spike){
              int spike_quantity = rand() % spike_quantity_max + 1;
              int spike_generation = generation_x + (rand() % rand_num) * 8 + 1;
                while(spike_generation + (spike_quantity - 1) * 5 > generation_x + (rand_num-1) * 8 + 2 && spike_generation > generation_x){
                  spike_generation -= 4;
                }
                while(spike_generation < generation_x){
                  spike_generation += 4;
                }
                while(spike_generation + (spike_quantity - 1) * 5 > generation_x + (rand_num-1) * 8 + 2){
                  spike_quantity--;
                }

              for(int i = 0; i < spike_quantity; i++)create_block(4, spike_generation + i * 5, (window_height - rand_y) - 5); 
            }
            //on ground
            //money 
            if((rand() & 31) > chance_money){
              create_block(2, generation_x + (rand() & 63), window_height - 12);
            }
            //spikes 
            if((rand() & 31) > chance_spike){
              int spike_quantity = rand() % spike_quantity_max + 1;
              int spike_generation = generation_x + (rand() & 31);
              for(int i = 0; i < spike_quantity; i++)create_block(4, spike_generation + i * 5, window_height - 9); 
            }
          generation_x += rand_num * 8 + (abs(rand()) & 31) + 8;
        }

        //block count test 
        /*int test = 0;
        for(int i = 0; i < 1000; i++){
          if(blocks[i] -> type != 0) test++;
        }
        
        std::cout << test << '\n';*/

        //input from keyboard
        while(SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT) running = false;
                if(event.type == SDL_KEYDOWN){
                    switch(event.key.keysym.sym){
                        case SDLK_SPACE:
                            player.jump();
                            break;
                    }
                }
                
        }
        if(true){
            if(keystate[SDL_SCANCODE_RIGHT]) player.right(delta);
            else if(keystate[SDL_SCANCODE_LEFT] && player.x > 3) player.left(delta);
            else player.stop(delta);
        }
         

        player.update(delta);
        for(int i = 0; i < 1000; i++){
          blocks[i]->update(delta);
        }

      
        if(player.x - cam_x > 15){
          cam_x += 10 * delta;
        }
        if(player.x - cam_x >= 50){
          cam_x = player.x - 50;
        }

        //painting
        //cleaning array of camera
        for(uint16_t i = 0; i + 6 < window_height; i++){
            for(uint16_t j = 0; j < window_width; j++){
                camera[i*window_width+j] = 2;
            }
        }
        for(uint16_t i = window_height - 6; i < window_height; i++){
            for(uint16_t j = 0; j < window_width; j++){
                camera[i*window_width+j] = 1;
            }
        }
        //player 
        if(player.direction)for(uint8_t i = 0; i < 6; i++){
            for(uint8_t j = 0; j < 6; j++)
            if(sprite_player[sprite_player_info[player.frame.first][0] + player.frame.second][i*6+j] != 255)camera[(int(player.y) - int(cam_y) + i)*window_width + (int(player.x) - int(cam_x) + j)] = sprite_player[sprite_player_info[player.frame.first][0] + player.frame.second][i*6+j];
        }
        else for(uint8_t i = 0; i < 6; i++){
            for(uint8_t j = 0; j < 6; j++)
            if(sprite_player[sprite_player_info[player.frame.first][0] + player.frame.second][i*6+5-j] != 255)camera[(int(player.y) - int(cam_y) + i)*window_width + (int(player.x) - int(cam_x) + j)] = sprite_player[sprite_player_info[player.frame.first][0] + player.frame.second][i*6+5-j];
        }
        
          //money "animation"
          if(anim_money_y - 4 * delta > 0){
            anim_money_direction = 0;
          }
          if(anim_money_y - 4 * delta < -3){
            anim_money_direction = 1;
          }
          anim_money_y += (int(anim_money_direction) * 2 - 1) * 4 * delta;
          //std::cout << anim_money_y << ' ' << anim_money_direction << '\n';
        //blocks 
        for(int l = 0; l < 1000; l++){
          switch(blocks[l]->type){
            //blocks 
            case 1:
              if(blocks[l]->x + 8< window_width + cam_x)
              for(int i = 0; i < 8; i++){
                for(int j = 0; j < 3; j++){
                  camera[(window_height - int(blocks[l]->y - int(cam_y) + j)) * window_width + int(blocks[l]->x - cam_x + i)] = 1;
                }
              }
              break;
            //money 
            case 2:
            case 3:
              if(blocks[l]->x + 5< window_width + cam_x)
              for(int i = 0; i < 5; i++){
                for(int j = 0; j < 5; j++){
                  if(money[i*5+j] != 255)camera[(int(blocks[l]->y - int(cam_y) + j + anim_money_y)) * window_width + int(blocks[l]->x - cam_x + i)] = money[i*5+j];
                }
              }
              break;
            //spikes 
            case 4:
              if(blocks[l]->x + 5 < window_width + cam_x)
              for(int i = 0; i < 3; i++){
                for(int j = 0; j < 5; j++){
                  if(spike[i*5+j] != 255){
                  camera[(int(blocks[l]->y - int(cam_y) + i)) * window_width + int(blocks[l]->x - cam_x + j)] = spike[i*5+j];
                  }
                }
              }
          }
        } 
        //amount of money
            for(int i = 0; i < 5; i++){
              for(int j = 0; j < 5; j++){
                if(money[i*5+j] != 255)camera[(i + 2) * window_width + j + 2] = money[i*5+j];
              }
            }
        

        std::string amount_money = std::to_string(money_collected);
        for(unsigned short l = 0; l < amount_money.size(); l++){
          for(int i = 0; i < 5; i++){
            for(int j = 0; j < 3; j++){
              if(numbers[amount_money[l] - '0'][i*3+j] == true){
                camera[window_width * 2 + l * 4 + i * window_width + j + 9] = 4;
              }
            }
          }
        }
        //std::cout << "money: " << amount_money << '\n';

        //paint x&y_near_block
        /*for(int i = 0; i < 8; i++){
          for(int j = 0; j < 3; j++){
            if(near_block_x != nullptr)camera[(window_height - int(near_block_x->y - int(cam_y) + j)) * window_width + int(near_block_x->x - cam_x + i)] = 0;
          }
        }*/ 
        /*for(int i = 0; i < 8; i++){
          for(int j = 0; j < 3; j++){
              if(near_block_y != nullptr)camera[(window_height - int(near_block_y->y - int(cam_y) + j)) * window_width + int(near_block_y->x - cam_x + i)] = 0;
          }
        }*/ 
        

        //convertion
        for(int i = 0; i < window_height * window_width; i++){
            final_screen[i] = pallete[camera[i]];
        }
        
        SDL_UpdateTexture(texture, NULL, final_screen, window_width * sizeof(uint16_t));
        SDL_RenderCopy(render, texture, NULL, NULL);
        SDL_RenderPresent(render);

        delta = float(SDL_GetTicks() - tick_start) / 1000;
        if(delta < 1.0f / 60.0f)SDL_Delay((1.0f / 60.0f - delta)*1000);
        delta = float(SDL_GetTicks() - tick_start) / 1000.0f;
        player.frame_time += delta;
        
        //fps test
        //std::cout << 1 / delta << "fps \n";
    }



    //end
    SDL_DestroyRenderer(render);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
