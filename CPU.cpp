// Implementation of CHIP-8 CPU

#include "CPU.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>

CPU::CPU(){}

CPU::~CPU(){};

void CPU::handleKeyPress(uint8_t key){

    /* Sets the appropriate index of h_KEYS on */

    h_KEYS[key] = 1;
}

void CPU::handleKeyRelease(uint8_t key){

    /* Sets the appropriate index of h_KEYS off */

    h_KEYS[key] = 0;
}

bool CPU::getPixelData(int x, int y){
    
    /* Returns a boolean value for the pixel at x,y */
    /* true if pixel is lit, false if pixel is not lit */
    
    return h_DISPLAY[x][y] ? true : false;
}

void CPU::reset(){
    
    /* Resets hardware and memory */
    
    h_PC = 0x200;
    h_I = 0;
    memset(h_REGISTERS, 0, sizeof(h_REGISTERS));
    memset(h_KEYS, 0, sizeof(h_KEYS));
    OPC_00E0(0);
}

void CPU::load(char* dir_game){
    
    /* Loads the game(ROM) and font into memory(h_RAM)*/

    // Loading game file
    FILE *in;
    in = fopen(dir_game, "rb");
    fread(&h_RAM[0x200], 0xFFF, 1, in);
    fclose(in);

    // Loading font into memory
    for(uint8_t i = 0; i<FONTSIZE; i++){
        h_RAM[i] = h_FONT[i];
    }
}

void CPU::lowerTimers(){

    /* Lowers internal timers (called at a frequency of 60hz) */

    if(h_DT > 0) h_DT--;
    if(h_ST > 0) h_ST--;
    if(h_ST > 0) playSound = true; // Does nothing right now
}

//void CPU::playSound(){}; // Does nothing right now

// Fetch
uint16_t CPU::readNextOpcode(){

    /* Gets and returns next opcode (16bits) */

    uint16_t opc = 0;
    opc = h_RAM[h_PC]; // Gets 8 bit first part of opcode
    opc <<= 8; // Bitshift to left by 8 bits
    opc |= h_RAM[h_PC+1]; // Use bitwise OR to get remaining 8 parts of opcode
    h_PC += 2; // Since we accessed 2x8 bits we increment PC by 2
    return opc;
}

// Decode -> decodes then calls appropriate opcode function to "execute" but for public uses, "executeOpcode" makes more sense
void CPU::executeOpcode(uint16_t opc){

    /* Decodes the opcode and calls the correct opcode function */

    switch (opc & 0xF000){
        case 0x0000:{
            switch(opc & 0x000F){
                case 0x0000: OPC_00E0(opc); break;
                case 0x000E: OPC_00EE(opc); break;
            } 
        }; break;
        case 0x1000: OPC_1NNN(opc); break;
        case 0x2000: OPC_2NNN(opc); break;
        case 0x3000: OPC_3XKK(opc); break;
        case 0x4000: OPC_4XKK(opc); break;
        case 0x5000: OPC_5XY0(opc); break;
        case 0x6000: OPC_6XKK(opc); break;
        case 0x7000: OPC_7XKK(opc); break;
        case 0x8000: {
            switch(opc & 0x000F){
                case 0x0000: OPC_8XY0(opc); break;
                case 0x0001: OPC_8XY1(opc); break;
                case 0x0002: OPC_8XY2(opc); break;
                case 0x0003: OPC_8XY3(opc); break;
                case 0x0004: OPC_8XY4(opc); break;
                case 0x0005: OPC_8XY5(opc); break;
                case 0x0006: OPC_8XY6(opc); break;
                case 0x0007: OPC_8XY7(opc); break;
                case 0x000E: OPC_8XYE(opc); break;
            }
        }; break;
        case 0x9000: OPC_9XY0(opc); break;
        case 0xA000: OPC_ANNN(opc); break;
        case 0xB000: OPC_BNNN(opc); break;
        case 0xC000: OPC_CXKK(opc); break;
        case 0xD000: OPC_DXYN(opc); break;
        case 0xE000:{
            switch(opc & 0x000F){
                case 0x000E: OPC_EX9E(opc); break;
                case 0x0001: OPC_EXA1(opc); break;
            }
        }; break;
        case 0xF000:{
            switch(opc & 0x000F){
                case 0x0007: OPC_FX07(opc); break;
                case 0x000A: OPC_FX0A(opc); break;
                case 0x0005: {
                    switch(opc & 0X00F0){
                        case 0x0010: OPC_FX15(opc); break;
                        case 0x0050: OPC_FX55(opc); break;
                        case 0x0060: OPC_FX65(opc); break;
                    }
                }; break;
                case 0x0008: OPC_FX18(opc); break;
                case 0x000E: OPC_FX1E(opc); break;
                case 0x0009: OPC_FX29(opc); break;
                case 0x0003: OPC_FX33(opc); break;
            }
        }; break;
        break;
        default: break; // opcode DNE or not yet implemented
    }

}

// Execute (Opcode functions)
void CPU::OPC_00E0(uint16_t opc){
    memset(h_DISPLAY, 0, sizeof(h_DISPLAY));
}

void CPU::OPC_00EE(uint16_t opc){
    h_PC = h_STACK.back();
    h_STACK.pop_back();
}

void CPU::OPC_1NNN(uint16_t opc){
    h_PC = opc & 0xfff;
}

void CPU::OPC_2NNN(uint16_t opc){
    h_STACK.push_back(h_PC);
    h_PC = opc & 0x0FFF;
}

void CPU::OPC_3XKK(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    uint8_t KK = opc & 0x00FF;
    h_REGISTERS[VX] == KK ? h_PC+=2 : h_PC+=0;  
}

void CPU::OPC_4XKK(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    uint8_t KK = opc & 0x00FF;
    h_REGISTERS[VX] != KK ? h_PC+=2 : h_PC+=0;  
}

void CPU::OPC_5XY0(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    uint8_t VY = (opc & 0x00F0) >> 4;
    h_REGISTERS[VX] == h_REGISTERS[VY] ? h_PC+=2 : h_PC+=0;
}

void CPU::OPC_6XKK(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    h_REGISTERS[VX] = opc & 0x00FF;
}

void CPU::OPC_7XKK(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    h_REGISTERS[VX] += opc & 0x00FF;
}

void CPU::OPC_8XY0(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    uint8_t VY = (opc & 0x00F0) >> 4;
    h_REGISTERS[VX] = h_REGISTERS[VY];
}

void CPU::OPC_8XY1(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    uint8_t VY = (opc & 0x00F0) >> 4;
    h_REGISTERS[VX] |= h_REGISTERS[VY];
}

void CPU::OPC_8XY2(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    uint8_t VY = (opc & 0x00F0) >> 4;
    h_REGISTERS[VX] &= h_REGISTERS[VY];
}

void CPU::OPC_8XY3(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    uint8_t VY = (opc & 0x00F0) >> 4;
    h_REGISTERS[VX] ^= h_REGISTERS[VY];
}

void CPU::OPC_8XY4(uint16_t opc){ // Double check overflow/underflow without using & to get lower 8 bits
    uint8_t VX = (opc & 0x0F00) >> 8;
    uint8_t VY = (opc & 0x00F0) >> 4;
    uint16_t sum = h_REGISTERS[VX] + h_REGISTERS[VY];
    if(sum > 0xFF){
        h_REGISTERS[VX] = sum & 0x00FF;
        h_REGISTERS[0xF] = 1;
    }
    else{
        h_REGISTERS[VX] = sum & 0x00FF;
        h_REGISTERS[0xF] = 0;
    }
}

void CPU::OPC_8XY5(uint16_t opc){ // Double check overflow/underflow without using & to get lower 8 bits
    uint8_t VX = (opc & 0x0F00) >> 8;
    uint8_t VY = (opc & 0x00F0) >> 4;
    uint16_t diff = h_REGISTERS[VX] - h_REGISTERS[VY];
    if(h_REGISTERS[VX] > h_REGISTERS[VY]){
        h_REGISTERS[VX] = diff & 0x00FF;
        h_REGISTERS[0xF] = 1;
    }
    else{
        h_REGISTERS[VX] = diff & 0x00FF;
        h_REGISTERS[0xF] = 0;
    }
}

void CPU::OPC_8XY6(uint16_t opc){ // Has two different "modes"
    uint8_t VX = (opc & 0x0F00) >> 8;
    //uint8_t VY = (opc & 0x00F0) >> 4;
    //h_REGISTERS[VX] = h_REGISTERS[VY]; // Either we have this step or ignore this step (depending on CHIP-8 version)
    h_REGISTERS[0xF] = h_REGISTERS[VX] & 0b00000001;
    h_REGISTERS[VX] >>= 1;
}

void CPU::OPC_8XY7(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    uint8_t VY = (opc & 0x00F0) >> 4;
    uint16_t diff = h_REGISTERS[VY] - h_REGISTERS[VX];
    if(h_REGISTERS[VY] > h_REGISTERS[VX]){
        h_REGISTERS[VX] = diff & 0x00FF;
        h_REGISTERS[0xF] = 1;
    }
    else{
        h_REGISTERS[VX] = diff & 0x00FF;
        h_REGISTERS[0xF] = 0;
    }
}

void CPU::OPC_8XYE(uint16_t opc){ // Has two different "modes"
    uint8_t VX = (opc & 0x0F00) >> 8;
    //uint8_t VY = (opc & 0x00F0) >> 4;
    //h_REGISTERS[VX] = h_REGISTERS[VY]; // Either we have this step or ignore this step (depending on CHIP-8 version)
    h_REGISTERS[0xF] = h_REGISTERS[VX] >> 7;
    h_REGISTERS[VX] <<= 1;
}

void CPU::OPC_9XY0(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    uint8_t VY = (opc & 0x00F0) >> 4;
    h_REGISTERS[VX] != h_REGISTERS[VY] ? h_PC+=2 : h_PC+= 0;
}

void CPU::OPC_ANNN(uint16_t opc){
    h_I = opc & 0x0FFF;
}

void CPU::OPC_BNNN(uint16_t opc){
    h_PC = opc & 0x0FFF;
    h_PC += h_REGISTERS[0];
}

void CPU::OPC_CXKK(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    uint8_t KK = (opc & 0X00FF);
    uint8_t rand_num = (rand() % 256) & KK;
}

void CPU::OPC_DXYN(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    uint8_t VY = (opc & 0x00F0) >> 4;

    uint8_t start_X = h_REGISTERS[VX];
    uint8_t start_Y = h_REGISTERS[VY];

    uint8_t N = opc & 0x000F;

    h_REGISTERS[0xF] = 0;

    for(uint8_t i = 0; i < N; i++){
        uint8_t factor_Y = i;
        uint8_t screen_Byte = h_RAM[h_I + i];

        uint8_t bit_mask = 1 << 7;

        for(uint8_t j = 0; j < 8; j++){
            uint8_t factor_X = j;
            if(start_X + j >= 64){
                uint8_t factor_X = start_X + j - 64;
            }

            uint8_t old_screen_Bit = h_DISPLAY[start_X + factor_X][start_Y + factor_Y];
            
            h_DISPLAY[start_X + factor_X][start_Y + factor_Y] ^= ((screen_Byte & bit_mask) >> (7-j));

            if (old_screen_Bit == 1 && h_DISPLAY == 0){
                h_REGISTERS[0xF] = 1;
            }
            bit_mask >>= 1;
        }
    }
}

void CPU::OPC_EX9E(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    if(h_KEYS[h_REGISTERS[VX]]){
        h_PC += 2;
    }
}

void CPU::OPC_EXA1(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    if(!h_KEYS[h_REGISTERS[VX]]){
        h_PC += 2;
    }
}

void CPU::OPC_FX07(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    h_REGISTERS[VX] = h_DT;
}

void CPU::OPC_FX0A(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    bool key_pressed = false;
    for(int i = 0; i < 16; i++){
        if (h_KEYS[i] > 0) key_pressed = true;
        h_REGISTERS[VX] = i;
    }
    if(!key_pressed) h_PC -= 2;
}

void CPU::OPC_FX15(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    h_DT = h_REGISTERS[VX];
}

void CPU::OPC_FX18(uint16_t opc){
    uint8_t VX = (opc & 0X0F00) >> 8;
    h_ST = h_REGISTERS[VX];
}

void CPU::OPC_FX1E(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    h_I += h_REGISTERS[VX];
}

void CPU::OPC_FX29(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    h_I = h_REGISTERS[VX] * 5;
}

void CPU::OPC_FX33(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    int num = h_REGISTERS[VX];
    h_RAM[h_I+2] = num % 10;
    h_RAM[h_I+1] = (num/10) % 10;
    h_RAM[h_I] = num / 100;
}

void CPU::OPC_FX55(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    for(int i = 0; i <= VX; i++){
        h_RAM[h_I + i] = h_REGISTERS[i];
    }
    //h_I += (VX + 1);  // Some implementations have this line of code on
}

void CPU::OPC_FX65(uint16_t opc){
    uint8_t VX = (opc & 0x0F00) >> 8;
    for(int i = 0; i <= VX; i++){
        h_REGISTERS[i] = h_RAM[h_I + i] ;
    }
    //h_I += (VX + 1);  // Some implementations have this line of code on
}