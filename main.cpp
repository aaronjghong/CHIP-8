#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <stdlib.h>
#include <stdio.h>
#include "CPU.h"
#include <fstream>
#include <windows.h>
#include <winuser.h>
#include <string>

// Setting constants
bool QUIT = false;

int settings[4];

#define SCREEN_WIDTH settings[0]
#define SCREEN_HEIGHT settings[1]
#define MAXOPCPERSECOND settings[2]
#define FPS settings[3]

std::string ERROR_CAPTION = "";
std::string ERROR_MESSAGE = "";

CPU* CHIP8CPU = new CPU();

// Create SDL window
SDL_Window *window = NULL;

// SDL Audio variables init
SDL_AudioSpec wavSpec;
Uint32 wavLength;
Uint8 *wavBuffer;
SDL_AudioDeviceID deviceId;

// To be put into external file soon
std::string GAMEDIR_STR; // = "C:\\Users\\Aaron Hong\\Desktop\\chip-8\\ROMS\\test_opcode.ch8";
char* GAMEDIR = &GAMEDIR_STR[0];

bool getSettings(){

    /* Initializes Emulator Settings from settings.txt */

    // Attempt to open settings file
    std::ifstream file("settings.txt");

    if(file.is_open()){
        // The file exists, proceed with reading the settings
        std::string line;
        int i = 0;
        while(getline(file, line)){
            // Get line, then parse for info using either // or \\ delimiters 
            int value = 0;
            if(line.find("\\\\") != std::string::npos){
                // Parse for Game Directory
                GAMEDIR_STR = line.substr(0, line.find("\\\\"));
                GAMEDIR = &GAMEDIR_STR[0];

                // Check if game directory is valid
                FILE *checkIfOpen = fopen(GAMEDIR, "r");
                if(!checkIfOpen){
                    // Game directory is not valid, return false
                    ERROR_CAPTION = "Game Not Found";
                    ERROR_MESSAGE = "Please check if the file in the game directory exists and that all file extentions are correct";
                    return false;
                }
                fclose(checkIfOpen);
                break; // Game directory is the last setting, it is okay to break loop
            }
            if(line.find("//") == std::string::npos){
                continue; // Skip instruction lines
            }
            try{
                // Checking if the settings input is valid
                if(!isdigit(line.substr(0, line.find("//"))[0])){
                    throw i;
                }
                value = std::stoi(line.substr(0, line.find("//")));
            }
            catch(int j){
                // Input was not valid, return false
                ERROR_CAPTION = "Settings Error";
                ERROR_MESSAGE = "There was an error with reading the settings at line " + std::to_string(j+3) + ".\nPlease check that all values are NUMERICAL and that the file is formatted correctly\nIf needed, please delete the settings file to reset the formatting";
                return false;
            }
            settings[i] = value;
            i++;
        }
    }
    else{
        // If the file does not exist, create settings.txt and return false
        std::ofstream newFile("settings.txt");
        newFile << "SETTINGS\nDO NOT DELETE ANYTHING, SIMPLY REPLACE THE VALUES NEXT TO THE DOUBLE SLASHES\n640//Screen Width\n320//Screen Height\n500//Max OPC Per Second\n60//FPS\nROMS\\GAME FILE\\\\Game Directory";
        newFile.close();
        ERROR_CAPTION = "First Run";
        ERROR_MESSAGE = "A file called settings.txt has been created in the same directory as the emulator\nPlease set the desired settings and re-open the application";
        return false;
    }

    // All settings initialized and valid, return true
    return true;
}

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

            // Play/Pause Audio
            if(CHIP8CPU->playSound) SDL_PauseAudioDevice(deviceId, 0);
            else SDL_PauseAudioDevice(deviceId, 1);

            // Set second time as reference time
            time1 = time2;
        }
    }
}

int main(int arg, char *argv[]){

    // Get Settings
    if(getSettings() == false){
        int alert = MessageBox(NULL, ERROR_MESSAGE.c_str(), ERROR_CAPTION.c_str(), MB_ICONERROR | MB_OK);
        return(3);
    }

    // Initialize SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ){
        return(1);
    }

    // Load sound into SDL
    if(SDL_LoadWAV("beep2.wav", &wavSpec, &wavBuffer, &wavLength) == NULL){
        ERROR_MESSAGE = "Error Loading Audio";
        ERROR_CAPTION = "Error";
        int alert = MessageBox(NULL, ERROR_MESSAGE.c_str(), ERROR_CAPTION.c_str(), MB_ICONERROR | MB_OK);
    }
    else{
        deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
        if(SDL_QueueAudio(deviceId, wavBuffer, wavLength) < 0){
            ERROR_MESSAGE = "Error Loading Audio";
            ERROR_CAPTION = "Error";
            int alert = MessageBox(NULL, ERROR_MESSAGE.c_str(), ERROR_CAPTION.c_str(), MB_ICONERROR | MB_OK);
        }
    }

    // Create window
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