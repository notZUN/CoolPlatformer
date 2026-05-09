#include <SDL2/SDL_timer.h>
#include <cstdint>
#include <SDL2/SDL.h>
#include <ctime>
#include "assets.h"

//create base variables for window width and hight, arrays for my "painting"(I don't know how call it), etc.
const uint8_t window_width = 128, window_height = 96;
uint8_t pixels[window_width * window_height];
std::pair<uint8_t, uint8_t> frame;
uint16_t pallete[256];
uint16_t final_screen[window_width * window_height];

int16_t x = 10, y = 50, xs = 0, ys = 0, fg = 1;



bool running, on_floor = 0, direction = 1;

int main(){
    srand(time(0));
    //create colors for pallete(I use RGB565, 255 is transparent)
    pallete[4] = 1983;
    pallete[3] = 31;
    pallete[2] = 63488;
    pallete[1] = 65471;
    pallete[0] = 0;
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
    
    running = 1;
    int tick_start, tick_time = -1, last_move = 0;

    //cleaning array of pixels
    for(uint16_t i = 0; i < window_height; i++){
            for(uint16_t j = 0; j < window_width; j++){
            pixels[i*window_width+j] = 0;
        }
    }
    
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
                            if(on_floor){
                                ys += 5;
                                on_floor = 0;
                            }
                    }
                }
                
        }
        if(tick_start - last_move >= 50){
            if(keystate[SDL_SCANCODE_RIGHT]) xs++;
            else if(keystate[SDL_SCANCODE_LEFT]) xs--;
            else if(xs > 0) xs--;
            else if(xs != 0) xs++;
        }

        //move(edit position, speed, etc.)
        if(tick_start - last_move >= 50){
            //player
            y -= ys;
            ys -= fg;
            if(xs > 3) xs = 3;
            else if(xs < -3) xs = -3;
            x += xs;
            if(x > 122) x = 122;
            if(x < 0) x = 0;
            if(y > 90) y = 90;
            if(y < 5) y = 5;
            if(y >= 90) on_floor = 1;
            if(on_floor) ys = 0;
            if(xs > 0) direction = 1;
            else if(xs < 0) direction = 0;

            //animation
            if(!on_floor){
                frame.first = 1;
                frame.second = 0;
            }
            else if(on_floor && xs != 0){
                if(frame.first != 0){
                    frame.first = 0;
                    frame.second = 0;
                }
                else{
                    frame.second = (frame.second + 1) % 3;
                }
            }
            else if(on_floor && xs == 0){
                frame.first = 0;
                frame.second = 0;
            }

            last_move = tick_start;
        }

    //paint
        //cleaning array
        for(uint16_t i = 0; i < window_height; i++){
            for(uint16_t j = 0; j < window_width; j++){
            pixels[i*window_width+j] = 0;
            }
        }

        //if direction is left(0) then copy of player sprite is revers, else normal copy(copy from sprites.h)
        if(!direction) for(uint8_t i = 0; i < 6; i++){
            for(uint8_t j = 0; j < 6; j++){
                if(sprite_player[frame.first][frame.second][i*6+5-j] != 255)pixels[(y+i)*window_width + (x+j)] = sprite_player[frame.first][frame.second][i*6+5-j];
            }
        }
        else for(uint8_t i = 0; i < 6; i++){
            for(uint8_t j = 0; j < 6; j++){
                if(sprite_player[frame.first][frame.second][i*6+j] != 255)pixels[(y+i)*window_width + (x+j)] = sprite_player[frame.first][frame.second][i*6+j];
            }
        }
        
        
        //copy from my array to array of final screen and use pallete for add colors
        for(int i = 0; i < window_width * window_height; i++){
            final_screen[i] = pallete[pixels[i]];
        }
        SDL_UpdateTexture(texture, NULL, final_screen, window_width * sizeof(uint16_t));
        SDL_RenderCopy(render, texture, NULL, NULL);
        SDL_RenderPresent(render);

        tick_time = SDL_GetTicks() - tick_start;
        if(tick_time < 16.67)SDL_Delay(16.67 - tick_time);
    }

    //end

    SDL_DestroyRenderer(render);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
