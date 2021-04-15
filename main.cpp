#include <SDL2/SDL.h>
// #include "SDL2/SDL_opengl.h"
#include <stdlib.h>
#include <stdio.h>
#include "CPU.h"

bool QUIT = false;

SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

CPU* CHIP8CPU = new CPU();

const int MAXOPCPERSECOND = 600;
const int FPS = 60;


std::string GAMEDIR_STR = "C:\\Users\\Aaron Hong\\Desktop\\chip-8\\ROMS\\UFO";
char* GAMEDIR = &GAMEDIR_STR[0];

bool createWindow(SDL_Surface* screenSurface){
    window = SDL_CreateWindow( "Chip 8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( !window ){
        return false;
    }
    else{
        //Get window surface
        screenSurface = SDL_GetWindowSurface(window);

        if( !screenSurface ){
            return false;
        }

        //Update the surface
        SDL_UpdateWindowSurface(window);

        //Wait two seconds
        //SDL_Delay(4000);

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

        return true;
    }
}

void getInput(){}

void drawScreen(){
    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
    uint32_t pixels[64*32];
    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 32; j++){
            SDL_Rect pixel;
            pixel.x = (SCREEN_WIDTH/64)*i;
            pixel.y = (SCREEN_HEIGHT/32)*j;
            pixel.w = (SCREEN_WIDTH/64);
            pixel.h = (SCREEN_HEIGHT/32);
            if(CHIP8CPU->getPixelData(i,j)){
                SDL_FillRect(screenSurface, &pixel, SDL_MapRGB(screenSurface->format, 255, 255, 255));
            }
            else{
                SDL_FillRect(screenSurface, &pixel, SDL_MapRGB(screenSurface->format, 0, 0, 0));
            }

            /*
            if(CHIP8CPU->getPixelData(i, j)){
                pixels[i*32 + j] = 0xFFFFFFFF;
            }
            else{
                pixels[i*32 + j] = 0x00000000;
            }

            Texture implementation
            */
        }
    }
    /*
    SDL_UpdateTexture(texture, NULL, pixels, 64*4);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    Texture Implementation
    */
    SDL_UpdateWindowSurface(window);
}

void doLoop(){
    CHIP8CPU->reset();
    CHIP8CPU->load(GAMEDIR);
    Uint32 time1 = SDL_GetTicks();
    SDL_Event event;
    int numOpcPerFrame = MAXOPCPERSECOND/FPS;
    while(!QUIT){
        // loop
        while(SDL_PollEvent(&event)){
            getInput();
            if(event.type == SDL_QUIT){
                return;
            }
        }

        Uint32 time2 = SDL_GetTicks();
        if(time1 + 1000 < time2){
            // 1 second has passed, perform numOpcPerFrame opcodes
            for(int i = 0; i < numOpcPerFrame; i++){
                uint16_t opc = CHIP8CPU->readNextOpcode();
                CHIP8CPU->executeOpcode(opc);
            }
            CHIP8CPU->lowerTimers();
            drawScreen();
            time1 = time2;
        }

    }
}


//The parameters in the main function cannot be omitted, or an error will be reported
int main(int arg, char *argv[]){

    //Initialize SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );

        return(1);
    }

    //Create window
    if(createWindow(surface) == false){
        return(2);
    }
    

    doLoop();

    // Free Surface
    //SDL_FreeSurface(surface);

    // Destroy Window
    SDL_DestroyWindow(window);

    SDL_DestroyRenderer(renderer);

    SDL_FreeSurface(surface);

    SDL_DestroyTexture(texture);


    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}