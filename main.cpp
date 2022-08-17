#include "src/chip8.h"
//Spot for openGl
#include "./src/Graphics/GraphicsClass.h"

int main(int argc, char** argv){
    Chip chip;

    //Graphics Setup:
    Graphics gfx;
    gfx.GraphicsRun();

    chip.Init();
    chip.LoadGame("Replace Name");

    for(;;){
        chip.EmulateChip();

        if(chip.draw_flag){
        //Draw graphics
        }

        chip.SetKeys();
    }

    return 0;
}