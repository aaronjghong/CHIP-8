#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "CPU.h"

// Setting constants
bool QUIT = false;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

CPU* CHIP8CPU = new CPU();

const int MAXOPCPERSECOND = 600;
const int FPS = 60;

// Create SDL window
SDL_Window *window = NULL;

// To be put into external file soon
std::string GAMEDIR_STR = "C:\\Users\\Aaron Hong\\Desktop\\chip-8\\ROMS\\test_opcode.ch8";
char* GAMEDIR = &GAMEDIR_STR[0];

bool createWindow(){
    
    /* Creates SDL window */
    
    window = SDL_CreateWindow( "Chip 8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( !window ){
        return false;
    }
    else{
        return true;
    }
}

void getInput(SDL_Event event){

    /* Gets the input from the SDL event and calls the appropriate key handler */

    if(event.type == SDL_KEYDOWN){
        switch(event.key.keysym.sym){
            case SDLK_1: CHIP8CPU->handleKeyPress(0); break;
            case SDLK_2: CHIP8CPU->handleKeyPress(1); break;
            case SDLK_3: CHIP8CPU->handleKeyPress(2); break;
            case SDLK_4: CHIP8CPU->handleKeyPress(3); break;
            case SDLK_q: CHIP8CPU->handleKeyPress(4); break;
            case SDLK_w: CHIP8CPU->handleKeyPress(5); break;
            case SDLK_e: CHIP8CPU->handleKeyPress(6); break;
            case SDLK_r: CHIP8CPU->handleKeyPress(7); break;
            case SDLK_a: CHIP8CPU->handleKeyPress(8); break;
            case SDLK_s: CHIP8CPU->handleKeyPress(9); break;
            case SDLK_d: CHIP8CPU->handleKeyPress(10); break;
            case SDLK_f: CHIP8CPU->handleKeyPress(11); break;
            case SDLK_z: CHIP8CPU->handleKeyPress(12); break;
            case SDLK_x: CHIP8CPU->handleKeyPress(13); break;
            case SDLK_c: CHIP8CPU->handleKeyPress(14); break;
            case SDLK_v: CHIP8CPU->handleKeyPress(15); break;
        }
    }
    else if (event.type == SDL_KEYUP){
        switch(event.key.keysym.sym){
            case SDLK_1: CHIP8CPU->handleKeyRelease(0); break;
            case SDLK_2: CHIP8CPU->handleKeyRelease(1); break;
            case SDLK_3: CHIP8CPU->handleKeyRelease(2); break;
            case SDLK_4: CHIP8CPU->handleKeyRelease(3); break;
            case SDLK_q: CHIP8CPU->handleKeyRelease(4); break;
            case SDLK_w: CHIP8CPU->handleKeyRelease(5); break;
            case SDLK_e: CHIP8CPU->handleKeyRelease(6); break;
            case SDLK_r: CHIP8CPU->handleKeyRelease(7); break;
            case SDLK_a: CHIP8CPU->handleKeyRelease(8); break;
            case SDLK_s: CHIP8CPU->handleKeyRelease(9); break;
            case SDLK_d: CHIP8CPU->handleKeyRelease(10); break;
            case SDLK_f: CHIP8CPU->handleKeyRelease(11); break;
            case SDLK_z: CHIP8CPU->handleKeyRelease(12); break;
            case SDLK_x: CHIP8CPU->handleKeyRelease(13); break;
            case SDLK_c: CHIP8CPU->handleKeyRelease(14); break;
            case SDLK_v: CHIP8CPU->handleKeyRelease(15); break;
        }
    }
}

void drawScreen(){

    /* Draws the screen from the data in h_DISPLAY*/

    // Get surface
    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 32; j++){
            
            // Set Rect dimensions according to window size/pixel position in 64x32 area
            SDL_Rect pixel;
            pixel.x = (SCREEN_WIDTH/64)*i;
            pixel.y = (SCREEN_HEIGHT/32)*j;
            pixel.w = (SCREEN_WIDTH/64);
            pixel.h = (SCREEN_HEIGHT/32);

            // Draw white if pixel data exists, else draw black
            if(CHIP8CPU->getPixelData(i,j)){
                SDL_FillRect(screenSurface, &pixel, SDL_MapRGB(screenSurface->format, 255, 255, 255));
            }
            else{
                SDL_FillRect(screenSurface, &pixel, SDL_MapRGB(screenSurface->format, 0, 0, 0));
            }
        }
    }
    // Update view
    SDL_UpdateWindowSurface(window);
    SDL_FreeSurface(screenSurface);
}

void doLoop(){

    /* Init CPU and go into emulator loop */

    // Reset CPU
    CHIP8CPU->reset();

    // Load Game
    CHIP8CPU->load(GAMEDIR);

    // Get Reference Time
    Uint32 time1 = SDL_GetTicks();

    SDL_Event event;
    int numOpcPerFrame = MAXOPCPERSECOND/FPS;

    while(!QUIT){
        // Poll for input
        while(SDL_PollEvent(&event)){
            getInput(event);
            if(event.type == SDL_QUIT){
                QUIT = true;
            }
        }
        
        // Get second time
        Uint32 time2 = SDL_GetTicks();
        if(time1 + (1000/FPS) < time2){
            // 1 Frame's worth of time has passed, perform numOpcPerFrame opcodes
            for(int i = 0; i < numOpcPerFrame; i++){
                uint16_t opc = CHIP8CPU->readNextOpcode();
                CHIP8CPU->executeOpcode(opc);
            }
            // Lower timers
            CHIP8CPU->lowerTimers();

            // Draw Frame
            drawScreen();

            // Set second time as reference time
            time1 = time2;
        }
    }
}

int main(int arg, char *argv[]){

    //Initialize SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );

        return(1);
    }

    //Create window
    if(createWindow() == false){
        return(2);
    }
    
    // Go into main emulator loop
    doLoop();

    // Destroy Window
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}