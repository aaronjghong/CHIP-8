#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>

const uint16_t GAMESIZE = 0xFFF;
const uint16_t FONTSIZE = 80;

class CPU{
    public:
                    ~CPU(){};
        void        load();
        void        reset();
        void        lowerTimers();
        uint8_t     readNextOpcode();                   // Likely not needed as public
        void        executeOpcode();
        void        handleKeyPress( uint8_t key );
        void        handleKeyRelease( uint8_t key );

        std::string dir_game_str = "C:\\Users\\Aaron Hong\\Desktop\\chip-8\\IBM Logo.ch8";
        char*       dir_game = &dir_game_str[0];        // Directory to game file in windows


    private:
                    CPU(){};
        void        playSound();

        uint8_t     h_RAM[GAMESIZE];                    // 0xFFF bytes (4096) of memory
        uint8_t     h_REGISTERS[16];                    // 16 1-byte registers
        uint16_t    h_PC;                               // 16 bit Program Counter
        uint16_t    h_I;                                // Index register
        std::vector<uint16_t> h_STACK;                  // Program Stack
        uint8_t     h_KEYS[16];                         // Keyboard
        uint8_t     h_DISPLAY[64][32];                  // Display Memory
        uint8_t     h_DT = 0;                           // Delay Timer (60hz countdown)
        uint8_t     h_ST = 0;                           // Sound Timer (60hz countdown)

        void        OPC_00E0( uint16_t opc );
        void		OPC_00EE( uint16_t opc );
        void		OPC_1NNN( uint16_t opc );
        void		OPC_2NNN( uint16_t opc );
        void		OPC_3XKK( uint16_t opc );
        void		OPC_4XKK( uint16_t opc );
        void		OPC_5XY0( uint16_t opc );
        void		OPC_6XKK( uint16_t opc );
        void		OPC_7XKK( uint16_t opc );
        void		OPC_8XY0( uint16_t opc );
        void		OPC_8XY1( uint16_t opc );
        void		OPC_8XY2( uint16_t opc );
        void		OPC_8XY3( uint16_t opc );
        void		OPC_8XY4( uint16_t opc );
        void		OPC_8XY5( uint16_t opc );
        void		OPC_8XY6( uint16_t opc );
        void		OPC_8XY7( uint16_t opc );
        void		OPC_8XYE( uint16_t opc );
        void		OPC_9XY0( uint16_t opc );
        void		OPC_ANNN( uint16_t opc );
        void		OPC_BNNN( uint16_t opc );
        void		OPC_CXKK( uint16_t opc );
        void		OPC_DXYN( uint16_t opc );
        void		OPC_EX9E( uint16_t opc );
        void		OPC_EXA1( uint16_t opc );
        void		OPC_FX07( uint16_t opc );
        void		OPC_FX0A( uint16_t opc );
        void		OPC_FX15( uint16_t opc );
        void		OPC_FX18( uint16_t opc );
        void		OPC_FX1E( uint16_t opc );
        void		OPC_FX29( uint16_t opc );
        void		OPC_FX33( uint16_t opc );
        void		OPC_FX55( uint16_t opc );
        void		OPC_FX65( uint16_t opc );


        uint8_t     h_FONT[FONTSIZE] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
};