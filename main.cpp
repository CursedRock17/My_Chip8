#include "src/chip8.h"
//Spot for openGl
#include "./src/Graphics/GraphicsClass.h"

#include <iostream>

int main(int argc, char** argv){
    if(argc < 2) {
        std::cout << "Chip8 Rom" << std::endl;
        return 1;
    }

    //Chip setup
    Chip chip;

    //Graphics setup:
    Graphics gfx;

    bool should_stop = false;

    chip.Init();
    chip.LoadGame(argv[1]);

    while(!should_stop){
        chip.EmulateChip();

        if(chip.draw_flag){
            int ender = gfx.GraphicsRun(chip);
            if(ender != 0)
                should_stop = true;
            chip.draw_flag = false;
        }
    }

    return 0;
}

