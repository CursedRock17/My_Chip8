#include "src/chip8.h"
//Spot for openGl
#include "./src/Graphics/GraphicsClass.h"

#include <iostream>

int main(int argc, char** argv){
    //Chip setup
    Chip chip;

    //Graphics setup:
    Graphics gfx;

    chip.Init();
    chip.LoadGame("Replace Name");


    bool should_stop = false;

    chip.LoadGame(argv[1]);

    while(!should_stop){
        chip.EmulateChip();

        if(chip.draw_flag){
            int ender = gfx.GraphicsRun(chip);
            if(ender != 0)
                should_stop = true;
        }

        
        chip.SetKeys();
    }

    return 0;
}