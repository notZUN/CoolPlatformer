#include <SDL2/SDL_timer.h>
#include <cstdint>
#include <SDL2/SDL.h>
#include <ctime>
#include "assets.h"

//create base variables for window width and hight, arrays for my "painting"(I don't know how call it), etc.
float delta = 0.016f;
const uint8_t fg = 100; //gravity
const uint8_t window_width = 128, window_height = 96;
uint8_t camera[window_width * window_height];
uint16_t final_screen[window_width * window_height];
uint16_t cam_x = 0, cam_y = 0;



int main(){
    srand(time(0));
    const uint8_t* keystate = SDL_GetKeyboardState(NULL);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Renderer* render = nullptr;

    //create window
    SDL_Window* window = SDL_CreateWindow(
        "Project_01",
        100, 
        100, 
        window_width * 10, 
        window_height * 10,
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

    
//game
    while(running){
        tick_start = SDL_GetTicks();
        SDL_RenderClear(render);

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


        //painting
        //cleaning array of camera
        for(uint16_t i = 0; i < window_height; i++){
            for(uint16_t j = 0; j < window_width; j++){
                camera[i*window_width+j] = 0;
            }
        }

        if(player.direction)for(uint8_t i = 0; i < 6; i++){
            for(uint8_t j = 0; j < 6; j++)
            if(sprite_player[sprite_player_info[player.frame.first][0] + player.frame.second][i*6+j] != 255)camera[(int(player.y) - cam_y + i)*window_width + (int(player.x) - cam_x + j)] = sprite_player[sprite_player_info[player.frame.first][0] + player.frame.second][i*6+j];
        }
        else for(uint8_t i = 0; i < 6; i++){
            for(uint8_t j = 0; j < 6; j++)
            if(sprite_player[sprite_player_info[player.frame.first][0] + player.frame.second][i*6+5-j] != 255)camera[(int(player.y) - cam_y + i)*window_width + (int(player.x) - cam_x + j)] = sprite_player[sprite_player_info[player.frame.first][0] + player.frame.second][i*6+5-j];
        }
        
        

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
    }



    //end
    SDL_DestroyRenderer(render);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
