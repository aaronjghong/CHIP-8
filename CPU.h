// Definitions for CHIP-8 CPU

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>

/* System Constants */
///////////////////////////////////////////////////////////////////////////////

const uint16_t GAMESIZE = 0xFFF;
const uint16_t FONTSIZE = 80;

class CPU{
    public:

        /* Public Functions */
        ///////////////////////////////////////////////////////////////////////

                                CPU();
                                ~CPU();
        void                    load( char* dir_game_str );         // Loads the game and font into memory (h_RAM)
        void                    reset();                            // Resets hardware and memory
        void                    lowerTimers();                      // Lowers internal timers (called at a frequency of 60hz)
        uint16_t                readNextOpcode();                   // Gets and returns next opcode (16bits)
        void                    executeOpcode( uint16_t opc );      // Decodes the opcode and calls the correct opcode function
        void                    handleKeyPress( uint8_t key );      // Sets the appropriate index of h_KEYS on
        void                    handleKeyRelease( uint8_t key );    // Sets the appropriate index of h_KEYS off
        bool                    getPixelData( int x, int y);        // Returns a boolean value for the pixel at x,y | true if pixel is lit, false if pixel is not lit 
        bool                    playSound;

    private:
                    
        //void                    playSound();                        // Not implemented yet

        /* Hardware */
        ///////////////////////////////////////////////////////////////////////

        uint8_t                 h_RAM[GAMESIZE];                    // 0xFFF bytes (4096) of memory
        uint8_t                 h_REGISTERS[16];                    // 16 1-byte registers
        uint16_t                h_PC;                               // 16 bit Program Counter
        uint16_t                h_I;                                // Index register
        std::vector<uint16_t>   h_STACK;                            // Program Stack
        uint8_t                 h_KEYS[16];                         // Keyboard
        uint8_t                 h_DISPLAY[64][32];                  // Display Memory
        uint8_t                 h_DT = 0;                           // Delay Timer (60hz countdown)
        uint8_t                 h_ST = 0;                           // Sound Timer (60hz countdown)

        /* Opcode Functions */
        ///////////////////////////////////////////////////////////////////////
        void                    OPC_00E0( uint16_t opc );
        void		            OPC_00EE( uint16_t opc );
        void		            OPC_1NNN( uint16_t opc );
        void		            OPC_2NNN( uint16_t opc );
        void		            OPC_3XKK( uint16_t opc );
        void		            OPC_4XKK( uint16_t opc );
        void		            OPC_5XY0( uint16_t opc );
        void		            OPC_6XKK( uint16_t opc );
        void		            OPC_7XKK( uint16_t opc );
        void		            OPC_8XY0( uint16_t opc );
        void		            OPC_8XY1( uint16_t opc );
        void		            OPC_8XY2( uint16_t opc );
        void		            OPC_8XY3( uint16_t opc );
        void		            OPC_8XY4( uint16_t opc );
        void		            OPC_8XY5( uint16_t opc );
        void		            OPC_8XY6( uint16_t opc );
        void		            OPC_8XY7( uint16_t opc );
        void		            OPC_8XYE( uint16_t opc );
        void		            OPC_9XY0( uint16_t opc );
        void		            OPC_ANNN( uint16_t opc );
        void		            OPC_BNNN( uint16_t opc );
        void		            OPC_CXKK( uint16_t opc );
        void		            OPC_DXYN( uint16_t opc );
        void		            OPC_EX9E( uint16_t opc );
        void		            OPC_EXA1( uint16_t opc );
        void		            OPC_FX07( uint16_t opc );
        void		            OPC_FX0A( uint16_t opc );
        void		            OPC_FX15( uint16_t opc );
        void		            OPC_FX18( uint16_t opc );
        void		            OPC_FX1E( uint16_t opc );
        void		            OPC_FX29( uint16_t opc );
        void		            OPC_FX33( uint16_t opc );
        void		            OPC_FX55( uint16_t opc );
        void		            OPC_FX65( uint16_t opc );

        /* Internal Font */
        ///////////////////////////////////////////////////////////////////////

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