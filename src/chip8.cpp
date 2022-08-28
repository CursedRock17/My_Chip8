#include "chip8.h"

Chip::Chip(){}
Chip::~Chip(){}

void Chip::EmulateChip(){
    //Fetch Opcode
    opcode = (memory[PC] << 8) | memory[PC + 1];
    //Move to next instruction before other stuff
    PC += 2;
    //It's easier to just premake these memory addresses

    std::uint16_t nnn = opcode & 0x0FFF; // lowest 12 bits
    std::uint8_t n = opcode & 0x000F; // lowest 4 bits
    std::uint8_t nn = opcode & 0x00FF; // lowest 8 bits
    std::uint16_t x = (opcode & 0x0F00) >> 8; // lower 4 bits of the high byte
    std::uint16_t y = (opcode & 0x00F0) >> 4; // upper 4 bits of the low byte

    //Decode Opcode - Convert from big endian to regular binary
    switch(opcode & 0xF000){
    // Some Opcodes //
    //We need to set I to the adress NNN by using ANNN
    //Execute opcodes - Go from current to what we need
    case 0x0000: 
        switch(opcode){ // Have to check the lower 4 bits after checking upper 4
            case 0x00E0:
            //Clear the screen
            //Replace all the values with 0, the hexcode for black
            graphics.fill(0);
            draw_flag = true;
                break;
            
            case 0x00EE: //Return from subroutine
            PC = stack[--sp];
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
        if(V[x] == nn){
            PC += 2;
        }
        break;
    
    case 0x4000:
        if(V[x] != nn){
            PC += 2;
        }
        break;

    case 0x5000: //if Vx == Vy skip the next instruction, jumping a code block
        if(V[x] == V[y])
            PC += 2;
        break;

    case 0x6000: //Just set Vx to nn
        V[x] = nn;
    
        break;

    case 0x7000: // Set Vx to nn + Vx or add nn
        V[x] += nn;
        
        break;

    case 0x8000:
        switch(n){
            case 0x0000: //Have to get to 0000 in binary (If & is exclusive), Set Vx to Vy, change those registries
            /*
            Explanation we have code 8XY0, the first one start with 8,
            we take all these 0x8XXX codes and AND with n aka 0x000F to get the lower 4 bits
            so 0x0008 = 1000000000000000 while 0x000F = 1111, the last 4 of 0x0008 are all 0
            since they;re not compatible the number becomes 0000 or this new case
            */
            V[x] = V[y];
                break;

            case 0x0001:  //Set Vx to Vx OR Vy
            V[x] |=  V[y];
                break;

            case 0x0002: //Set Vx to Vx AND Vy
            V[x] &= V[y];
                break;

            case 0x0003: //Set Vx to Vx XOR Vy
            V[x] ^= V[y];
                break;

            case 0x0004: // Set Vx to Vx + Vy, then must set Vf to carry;
            V[x] += V[y];
            if((V[x]) > 0x00FF) //Because we do the math beforehand we can check V[x]
                //We need to set VF (F is a hexadecimal) to 1, also known as a carry
                V[0xF] = 1;
            else {
                //Otherwise just set VF to 0
                V[0xF] = 0;
            }
                break;

            case 0x0005: //Set Vx to Vx - Vy, if Vx > Vy we need to borrow
            if(V[x] > V[y])
                V[0xF] = 1;
            else {
                V[0xF] = 0;
            }
            V[x] -= V[y];
                break;

            case 0x0006: //Store the least signifcant bit of Vx in Vf then shifts Vx to the right 1
            if((V[x] & 1) == 1)
                V[0xF] = 1;
            else 
                V[0xF] = 0;
            
            V[x] >>= 1;
                break;

            case 0x0007: // Set Vx to Vy - Vx, if Vy > Vx then there's a borrow, we can just check in reverse order
            if (V[y] > V[x])
                V[0xF] = 1;
            else 
                V[0xF] = 0;

            V[x] = V[y] - V[x];
                break;

            case 0x000E: //Stores the most significnt bit of Vx in Vf then shifts Vx left 1, the opposit of 8XY6
            if((V[x] & 7) == 1)
                V[0xF] = 1;
            else 
                V[0xF] = 0;
            V[x] <<= 1; //This also counts as multiplication by 2 for example:
            /*
            0011 = 3 if you move it the left 1, you get 0110
            or 0420 in its place which is 6. 3 * 2 = 6
            */
                break;
            default:
                std::cout << "Unknown opcode" << opcode << std::endl;
        }
        break;

    case 0x9000: // If Vx != Vy skip the next instruction
    if(V[x] != V[y]){
        PC += 2;
    }
        break;

    case 0xA000: //This is ANNN, the start of the letters, it sets I to nnn, it turns to hex instead of numbers
    I = nnn;   
        break;

    case 0xB000: // Jump to location nnn + V0 then set the program counter to that location
    PC = nnn + V[0x0];
        break;

    case 0xC000: { //Set Vx to random byte AND nn, need a random number for the byte between 0 to 255
    srand(time(0));
    int rand_byte = rand()% 255;
    //This switch case needs brackets becyase it has private variables in the scope
    V[x] = nn & rand_byte;
        break;
    }

    case 0xD000: //Draw function, starting a memory location (Vx, Vy)
    {            
            uint8_t Vx = V[x];
            uint8_t Vy = V[y];
            uint8_t pixel;
            V[0xF] = 0;
            for(int yline = 0; yline < n; yline++) {
                pixel = memory[I + yline];
                for(int xline = 0; xline < 8; xline++) {
                    if (0 != (pixel & (0x80 >> xline))) {
                        if (1 == graphics[Vx + xline + ((Vy + yline) * 64)]) {
                            V[0xF] = 1;
                        }
                        graphics[Vx + xline + ((Vy + yline) * 64)] ^= 1;
                    }
                }
            }
            draw_flag = true;
        break;
    }

    case 0xE000:{
        switch(nn){
            //Ex9e skips the next instruction if
            //the key stored in VX is pressde
            case 0x009E:
                if(key[V[x]] != 0)
                    PC += 2;
        
                break;

            case 0x00A1: //Checks if V[x] is in the up position, if it is PC += 2
                if(key[V[x]] == 1)
                    PC += 2;
                break;

            default: 
                std::cout << "Unkown Opcode [0x0000]: " << opcode << std::endl;
        }
        break;
    }
    

    case 0xF000:{
        switch(nn){
            case 0x0007: // Set Vx to the delay timer value
            V[x] = delay_timer;
                break;

            case 0x000A: //Store the value of a key press in Vx
            {
                
            bool has_pressed = true;
                for(const auto& each_key : key){
                    if(0 != each_key){
                        V[x] = I;
                        has_pressed = false;
                    }
            }

            if(has_pressed){
                break;
            }
                break;
            }

            case 0x0015: //Set Delay Timer to Vx
            delay_timer = V[x];
                break;

            case 0x0018: // Set sound timer to Vx
            sound_timer = V[x];
                break;

            case 0x001E: // Add I and Vx into I
            V[0xF] = (I + V[x] > 0xFFF) ? 1 : 0;
            I += V[x];
                break;

            case 0x0029: // I equals to the location of sprtite for digit Vx
            I = V[x] * 0x5;
                break;

            case 0x0033: //0xFX33
                memory[I] = V[x] / 100;
                memory[I + 1] = (V[x] / 10) % 10;
                memory[I + 2] = (V[x] / 100) % 10;
                break;

            case 0x0055: //Store registers V0 through Vx in memory starting at I
            for(int register_index = 0; register_index < 16; register_index++){
                memory[I + register_index] = V[register_index];
            }
                break;

            case 0x0065: // Read registers V0 through Vx from memory starting at I
            for(int mem_index = 0; mem_index < 16; mem_index++){
                V[mem_index] = memory[I + mem_index];
            }
                break;

            default: 
                std::cout << "Unkown Opcode [0x0000]: " << opcode << std::endl;
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

        default: 
            std::cout << "Unkown Opcode [0x0000]: " << opcode << std::endl;
    }

    std::cout << PC  << "  " << I << std::endl;
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

    memory.fill(0);
    graphics.fill(0);
    key.fill(0);

    //Load Fontset
    for(int i = 0; i < 80; ++i){
       memory[i] = fontset[i];
    }
    //Reset Timers
    sound_timer = 0;
    delay_timer = 0;
    draw_flag = false;

    srand(time(NULL));
}

void Chip::LoadGame(const char* game_name){
    std::ifstream game_file(game_name, std::ios::binary | std::ios::ate);
    std::ifstream::pos_type file_size = game_file.tellg();
    std::vector<std::uint8_t> buffer(file_size);
    game_file.seekg(0, std::ios_base::beg);
    game_file.read(reinterpret_cast<char*>(buffer.data()), file_size);


    if(file_size > 4096 - 512)
        std::cout << "Issue, ROM file too large: " << file_size << std::endl;
    else {
    //load program into memory starting at 512 or 0x200
    //Need to maksure it can fit as well
        for(int i = 0; i < file_size; ++i){
            memory[i + 512] = buffer[i];
        }
    }

}

void Chip::SetKeys(int hex){
    if(key[hex] == 0){
        key[hex] = 1;
        std::cout << "Press: " << opcode << std::endl;
    }

    if(key[hex] == 1){
        key[hex] = 0;
        std::cout << "Release: " << opcode << std::endl;
    }
}