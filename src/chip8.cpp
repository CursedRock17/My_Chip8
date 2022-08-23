#include "chip8.h"

Chip::Chip(){}
Chip::~Chip(){}

void Chip::EmulateChip(){
    
    //Fetch Opcode
    opcode = memory[PC] << 8 | memory[PC + 1];
    //It's easier to just premake these memory addresses

    uint16_t nnn = opcode & 0x0FFF; // lowest 12 bits
    uint8_t n = opcode & 0x000F; // lowest 4 bits
    uint8_t nn = opcode & 0x00FF; // lowest 8 bits
    uint8_t x = opcode >> 8 & 0x000F; // lower 4 bits of the high byte
    uint8_t y = opcode >> 4 & 0x000F; // upper 4 bits of the low byte

    //Decode Opcode - Convert from big endian to regular binary
    switch(opcode & 0xF000){
    // Some Opcodes //

    //We need to set I to the adress NNN by using ANNN
    //Execute opcodes - Go from current to what we need
    case 0x0000: 
        switch(n){ // Have to check the lower 4 bits after checking upper 4
            case 0x0000:
            //Clear the screen
            //Replace all the values with 0, the hexcode for black
            graphics.fill(0);
            draw_flag = true;
            PC += 2;
            break;
            
            case 0x000E: //Return from subroutine
            sp--;
            PC = stack[sp];
            PC += 2;
            break;

            default: 
                std::cout << "Unkown Opcode [0x0000]: " << opcode << std::endl;
        }
    break;

    //More opcodes - Probably going to move in list order
    //They're already in big endian just remove the non-hex letters

    case 0x1000: //1NNN - Jump to address NNN which is always opcode 0x0FFF
        PC = nnn;
        break;

    case 0x2000:
    //Call the subroute at NNN
        ++sp;
        stack[sp] = PC;
        PC = nnn;
        break;

    case 0x3000: //Skip the next instruction if Vx = NN, normally a jump to the next code block
        if(V[x] == V[nn]){
            //V[0xF] = 1; //Carry
            PC += 4;
        }
        else 
            PC += 2;
        break;
    
    case 0x0004:
        if(V[x] != V[nn]){
            PC += 2;
        }

        PC += 2;
        break;

    case 0x0005: //if Vx == Vy skip the next instruction, jumping a code block
        if(V[x] == V[y])
            PC += 4;
        else 
            PC += 2;

        break;


    case 0x0006: //Just set Vx to nn
        V[x] = nn;
        PC++;

        break;

    case 0x0007: // Set Vx to nn + Vx or add nn
        V[x] += nn;

        PC += 4;
        break;

    case 0x0008:
        switch(n){
            case 0x0000: //Have to get to 0000 in binary (If & is exclusive), Set Vx to Vy, change those registries
            /*
            Explanation we have code 8XY0, the first one start with 8,
            we take all these 0x8XXX codes and AND with n aka 0x000F to get the lower 4 bits
            so 0x0008 = 1000000000000000 while 0x000F = 1111, the last 4 of 0x0008 are all 0
            since they;re not compatible the number becomes 0000 or this new case
            */
            V[x] = V[y];

            PC += 2;
            break;

            case 0x0001:  //Set Vx to Vx OR Vy
            V[x] |=  V[y];

            PC += 2;
            break;

            case 0x0002: //Set Vx to Vx AND Vy
            V[x] &= V[y];

            PC += 2;
            break;

            case 0x0003: //Set Vx to Vx XOR Vy
            V[x] ^= V[y];

            PC += 2;
            break;

            case 0x0004: // Set Vx to Vx + Vy, then must set Vf to carry;
            V[x] += V[y];

            if((V[x]) > 0x00FF) //Because we do the math beforehand we can check V[x]
                //We need to set VF (F is a hexadecimal) to 1, also known as a carry
                V[0xF] = 1;
            else 
                //Otherwise just set VF to 0
                V[0xF] = 0;
            PC += 2;

            break;

            case 0x0005: //Set Vx to Vx - Vy, if Vx > Vy we need to borrow
            if(V[x] > V[y])
                V[0xF] = 1;
            else 
                V[0xF] = 0;
            V[x] -= V[y];

            PC += 2;

            break;

            case 0x0006: //Store the least signifcant bit of Vx in Vf then shifts Vx to the right 1
            if((V[x] & 1) == 1)
                V[0xF] = 1;
            else 
                V[0xF] = 0;
            
            V[x] >>= 1;
            PC += 2;

            break;

            case 0x0007: // Set Vx to Vy - Vx, if Vy > Vx then there's a borrow, we can just check in reverse order
            if (V[y] > V[x])
                V[0xF] = 1;
            else 
                V[0xF] = 0;

            V[x] = V[y] - V[x];
            PC += 2;

            break;

            case 0x0008: //Stores the most significnt bit of Vx in Vf then shifts Vx left 1, the opposit of 8XY6
            if((V[x] & 7) == 1)
                V[0xF] = 1;
            else 
                V[0xF] = 0;
            
            V[x] <<= 1; //This also counts as multiplication by 2 for example:
            /*
            0011 = 3 if you move it the left 1, you get 0110
            or 0420 in its place which is 6. 3 * 2 = 6
            */
            PC += 2;
            break;
        }

        break;

    case 0x0009: // If Vx != Vy skip the next instruction
    if(V[x] != V[y])
        PC += 4;
    else 
        PC += 2;
    break;

    case 0xA000: //This is ANNN, the start of the letters, it sets I to nnn, it turns to hex instead of numbers
    I = nnn;   
    PC += 2; 

    break;

    case 0xB000: // Jump to location nnn + V0 then set the program counter to that location
    PC = nnn + V[0x0];

    break;

    case 0xC000: { //Set Vx to random byte AND nn, need a random number for the byte between 0 to 255
    srand(time(0));
    int rand_byte = rand()% 255;
    //This switch case needs brackets becyase it has private variables in the scope
    V[x] = nn & rand_byte;
    PC += 2;

    break;
    }

    case 0xD000: //Draw function
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

    case 0xE000:{
        switch(opcode & 0x00FF){
            //Ex9e skips the next instruction if
            //the key stored in VX is pressde
            case 0x009E:
                if(key[V[x]] != 0)
                    PC += 4;
                else 
                    PC += 2;
            break;

            case 0x00A1: //Checks if V[x] is in the up position, if it is PC += 2
                if(key[V[x]] == 1)
                    PC += 4;
                else 
                    PC += 2;
            break;
        }
    break;
    }
    

    case 0xF000:{
        switch(opcode & 0x00FF){
            case 0x0007: // Set Vx to the delay timer value
            V[x] = delay_timer;
            PC += 2;
            break;

            case 0x000A: //Store the value of a key press in Vx
            {
            bool has_pressed = false;
                for(const auto& each_key : key){
                    if(0 != each_key){
                        V[x] = I;
                        has_pressed = true;
                    }
            }

            if(!has_pressed)
                return;
            PC += 2;
            }
            break;

            case 0x0015: //Set Delay Timer to Vx
            delay_timer = V[x];

            PC += 2;
            break;

            case 0x0018: // Set sound timer to Vx
            sound_timer = V[x];

            PC += 2;
            break;

            case 0x001E: // Add I and Vx into I
            V[0xF] = (I + V[x] > 0xFFF) ? 1 : 0;
            I += V[x];

            PC += 2;
            break;

            case 0x0029: // I equals to the location of sprtite for digit Vx
            I = V[x] * 0x5;

            PC += 2;
            break;

            case 0x0033: //0xFX33
                memory[I] = V[x] / 100;
                memory[I + 1] = (V[x] / 10) % 10;
                memory[I + 2] = (V[x] / 100) % 10;
                PC += 2;
            break;

            case 0x0055: //Store registers V0 through Vx in memory starting at I
            for(int register_index = 0; register_index < 16; register_index++){
                memory[I + register_index] = V[register_index];
            }

            PC += 2;
            break;

            case 0x0065: // Read registers V0 through Vx from memory starting at I
            for(int mem_index = 0; mem_index < 16; mem_index++){
                V[mem_index] = memory[I + mem_index];
            }

            PC += 2;
            break;
        }
        break;
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

    graphics.fill(0);

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

int Chip::KeyPressed(){
    return 0;
}

void Chip::LoadGame(const char* game_name){
    std::cout << game_name << std::endl;
    Init(); //Need to reiniziale to clear everythign

    FILE* game_file = fopen(game_name, "file");


    //load program into memory
    /*
    for(int j = 0; j < 4000; ++j){
        memory[j + 512] = buffer[j];
    }
    */

   //make sure it's not too big
    fclose(game_file);
}

void Chip::SetKeys(){

}