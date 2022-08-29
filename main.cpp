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

    chip.Init();
    chip.LoadGame(argv[1]);

    while(!gfx.Needs_Stop()){
        chip.EmulateChip();

        if(chip.draw_flag){
            gfx.GraphicsRun(chip);
            gfx.Add_Delay();
            chip.draw_flag = false;
        }

        gfx.Check_Keys(chip);
    }

    return 0;
}

