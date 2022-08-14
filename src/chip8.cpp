#include "chip8.h"

Chip::Chip(){}
Chip::~Chip(){}

void Chip::EmulateChip(){
    //Fetch Opcode
    memory[PC] = 0xA2;
    memory[PC + 1] = 0xF0;

    opcode = memory[PC] << 8 | memory[PC + 1];

    //Decode Opcode

    switch(opcode & 0xF000){
    // Some Opcodes //

    //We need to set I to the adress NNN by using ANNN
    case 0xA000: 
        I = opcode & 0x0FFF;
        PC += 2;
    break;

    //More opcodes

    }

    //Execute

    //Set Timers

    sound_timer = 60;
    delay_timer = 60;
}

void Chip::Init(){
    //Begin loading the game and prepping the registers
    opcode = 0;
    PC = 0x200;
    I = 0;
    sp = 0;

    //Clear Display, stack, registers, memory
    for(int i = 0; i < 80; ++i){
    //Load Fontset
        memory[i] = fontset[i];

    //Reset Timers
    sound_timer = 0;
    delay_timer = 0;
    }


    //load program into memory
    for(int j = 0; j < 4000; ++j){
        memory[j + 512] = buffer[j];
    }

}

void Chip::LoadGame(const char* game_name){

}

void Chip::SetKeys(){

}