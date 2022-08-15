#include "chip8.h"

Chip::Chip(){}
Chip::~Chip(){}

void Chip::EmulateChip(){
    //Fetch Opcode
    opcode = memory[PC] << 8 | memory[PC + 1];

    //Decode Opcode - Convert from big endian to regular binary
    switch(opcode & 0xF000){
    // Some Opcodes //

    //We need to set I to the adress NNN by using ANNN
    //Execute opcodes - Go from current to what we need
    case 0x0000: 
        switch(opcode & 0x000F){
            case 0x0000:
            //Clear the screen
            //I = opcode & 0x0FFF;
            //PC += 2;
            break;
            
            case 0x000E: //Return from subroutine

            break;

            default: 
                std::cout << "Unkown Opcode [0x0000]: " << opcode << std::endl;
        }
    break;

    //More opcodes

    case 0x2000:
    //Call the subroute at NNN
        stack[sp] = PC;
        ++sp;
        PC = opcode & 0x0FFF;
        break;
    
    case 0x0004:
        if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
            V[0xF] = 1; //Carry

        else 
            V[0xF] = 0;

        V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
        PC += 2;
        break;


    case 0x0033: //0xFX33
        memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
        memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
        memory[I + 2] = (V[(opcode & 0x0F00) >> 8] / 100) % 10;
        PC += 2;
        break;

    case 0xD000:
    {
        unsigned short x = V[(opcode & 0x0F00 >> 8) >> 8];
        unsigned short y = V[(opcode & 0x00F0) >> 4];
        unsigned short height = opcode & 0x000F;
        unsigned short pixel;

        V[0xF] = 0;
        for(int yLine = 0; yLine < height; yLine++){
            pixel = memory[I + yLine];
            for(int xline = 0; xline < 8; xline++){
                if((pixel * (0x80 >> xline)) != 0){
                    if(graphics[(x + xline + (y + yLine) * 64)] == 1)
                        V[0xF] = 1;
                    graphics[x + xline + (y + yLine * 64)] ^= 1; 
                }
            }
        }

    draw_flag = true;
    PC += 2;

    }
    break;

    case 0xE000:
        switch(opcode & 0x00FF){
            //Ex9e skips the next instruction if
            //the key stored in VX is pressde
            case 0x009E:
                if(key[V[(opcode & 0x0F00) >> 8]] != 0)
                    PC += 4;
                else 
                    PC += 2;
            break;
        }

    }


    //Set Timers
    if(delay_timer > 0)
        --delay_timer;

    if(sound_timer > 0) {
        if(sound_timer == 1)
            std::cout << "Beep" << std::endl;
        --sound_timer;
    }
}

void Chip::Init(){
    //Begin loading the game and prepping the registers
    opcode = 0;
    PC = 0x200;
    I = 0;
    sp = 0;

    //Clear Display, stack, registers, memory
    std::fill_n(V, 16, 0);

    for(int sc = 0; sc < 16; sc++){
        stack[sc] = 0;
    }

    std::fill_n(memory, 4096, 0);

    std::fill_n(graphics, (64 * 32), 0);

    std::fill_n(key, 16, 0);

    for(int i = 0; i < 80; ++i){
    //Load Fontset
        memory[i] = fontset[i];

    //Reset Timers
    sound_timer = 0;
    delay_timer = 0;

    draw_flag = true;
    }


}

void Chip::LoadGame(const char* game_name){
    FILE* game_file = fopen(game_name, "file");

    //load program into memory
    /*
    for(int j = 0; j < 4000; ++j){
        memory[j + 512] = buffer[j];
    }
    */

   //make sure it's not too big
}

void Chip::SetKeys(){

}