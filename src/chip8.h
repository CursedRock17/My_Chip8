#ifndef CHIP8_H
#define CHIP8_H

#include <array>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

class Chip {
public:

Chip();
~Chip();

void EmulateChip();
void Init();
void LoadGame(const char* game_name);

void SetKeys(int index, int power);

bool draw_flag = false;

std::array<unsigned char,(64 * 32)> graphics; //Replace with SDL or openGL

private:

//This Memory starts arter 0x200 (512) thats where the interpreter is
std::array<unsigned short, 4096> memory;

unsigned short opcode{}; 

unsigned char sound_timer = {0};
unsigned char delay_timer = {0};

std::array<unsigned short, 16> stack;
unsigned char sp = {0}; //Stack Pointer

//Keyboard
std::array<unsigned short, 16> key;

//Registers sections need a few types in the opcode table: NNN, NN, N, X, Y, PC, I, VN
unsigned char V[16];
unsigned short PC = {0x200};
unsigned short I{};


//Keypad is made of 16 registers for the Vx and maps 0-9 and A-F, its a 4x4 matrix
std::array<unsigned char, 80> fontset =
{ 
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

#endif