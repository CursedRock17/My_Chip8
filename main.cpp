#include "src/chip8.h"
//Spot for openGl
#include <GLFW/glfw3.h>

int main(int argc, char** argv){
    Chip chip;

    //Graphics Setup:
    

    chip.Init();
    chip.LoadGame("Name");

    for(;;){
        chip.EmulateChip();

        if(chip.draw_flag){
        //Draw graphics
        }

        chip.SetKeys();
    }

    return 0;
}