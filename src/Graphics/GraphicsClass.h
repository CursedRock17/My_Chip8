#ifndef GRAPHICSCLASS_h
#define GRAPHICSCLASS_h

#include "../chip8.h"

#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>

class Graphics {
public:
    Graphics();
    ~Graphics();

    int GraphicsRun(Chip chip);
    void Add_Delay();
    void Check_Keys(Chip chip);
    bool Needs_Stop();

private:

    void GraphicsUpdate(const Chip& c8);

    //Have these work in tandem, it's a good pracetice to have a class deal with them automatically
    void GraphicsInit();
    void GraphicsTerminate();

    //----> Initiailize with the first 5 lines of code
    // These are SDL specific
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Rect pixel;

    // -----> These two will control how the window will run
    SDL_Event event;
    bool should_stop = false;

    //Creating the Texure
    const unsigned short img_width = 64;
    const unsigned short img_height = 32; //Unsigned short is 16 bits
    const std::uint16_t modifier = 15;

    //General Window Uses
    int WINDOW_HEIGHT = img_height * modifier;
    int WINDOW_WIDTH = img_width * modifier;
    int PIXEL_SIZE = 14;
};

#endif