#include "GraphicsClass.h"
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>

Graphics::Graphics()
{
    //Create the basic window
    GraphicsInit();
}

Graphics::~Graphics()
{
    //Good Code practice to have things ahndled automatically
    GraphicsTerminate();
}

/*SDL
*/
int Graphics::GraphicsRun(Chip chip)
{
    //Need to clear the colors and make sure we start by drawing on a black backgrfound
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
	SDL_RenderClear(renderer);

    //Go through the size of the graphics
    GraphicsUpdate(chip);

    SDL_RenderPresent(renderer);
    //Check to make sure the window needs to stay up

    return 0;
}

void Graphics::GraphicsInit()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_RENDERER_ACCELERATED);
    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
    SDL_RenderClear(renderer);

    //Pixel Size will always be the same, no need to change it
    // -----> Create a pixel in the shape of a square and initialize it's size
    pixel.h = PIXEL_SIZE;
    pixel.w = PIXEL_SIZE;
}

void Graphics::GraphicsTerminate()
{
    //Simple destroy loop
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void Graphics::GraphicsUpdate(const Chip& chip)
{
        for(int hi = 0; hi < 32; ++hi){
            for(int wi = 0; wi < 64; ++wi){
                //We need to update this to make sure all the pixels will fit on the screen after being adjusted in size
                pixel.x = wi * 15;
                pixel.y = hi * 15;

                //Check if the current point in graphics needs to turn black,
                //We then chnage the color beforehand then draw at the end of the command
				//Had to make sure that we can access every single element in graphics
                if(chip.graphics[hi*64 + wi] == 0){
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1); //Black
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1); //White
                }

                SDL_RenderDrawRect(renderer, &pixel);
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
}

void Graphics::Add_Delay()
{
    SDL_Delay(10);
}

void Graphics::Check_Keys(Chip chip)
{
    while(SDL_PollEvent(&event)){
    if(event.type == SDL_QUIT)
        {
            should_stop = true;
        }
    else if(event.type == SDL_KEYDOWN)
					{
						//Select surfaces based on key press
						switch(event.key.keysym.sym)
						{
						case SDLK_x:
                            chip.SetKeys(0x0, 1);
							break;

						case SDLK_1:
							chip.SetKeys(0x1, 1);
							break;

						case SDLK_2:
							chip.SetKeys(0x2, 1);
							break;

						case SDLK_3:
							chip.SetKeys(0x3, 1);
							break;

						case SDLK_q:
							chip.SetKeys(0x4, 1);
							break;

						case SDLK_w:
							chip.SetKeys(0x5, 1);
							break;

						case SDLK_e:
							chip.SetKeys(0x6, 1);
							break;

						case SDLK_a:
							chip.SetKeys(0x7, 1);
							break;

						case SDLK_s:
							chip.SetKeys(0x8, 1);
							break;

						case SDLK_d:
							chip.SetKeys(0x9, 1);
							break;

						case SDLK_z:
							chip.SetKeys(0xA, 1);
							break;

						case SDLK_c:
							chip.SetKeys(0xB, 1);
							break;

						case SDLK_4:
							chip.SetKeys(0xC, 1);
							break;

						case SDLK_r:
							chip.SetKeys(0xD, 1);
							break;

						case SDLK_f:
							chip.SetKeys(0xE, 1);
							break;

						case SDLK_v:
							chip.SetKeys(0xF, 1);
							break;

						}
					}
					if(event.type == SDL_KEYUP)
					{
						//Select surfaces based on key press
						switch(event.key.keysym.sym)
						{
						case SDLK_x:
							chip.SetKeys(0x0, 0);
							break;

						case SDLK_1:
							chip.SetKeys(0x1, 0);
							break;

						case SDLK_2:
							chip.SetKeys(0x2, 0);
							break;

						case SDLK_3:
							chip.SetKeys(0x3, 0);
							break;

						case SDLK_q:
							chip.SetKeys(0x4, 0);
							break;

						case SDLK_w:
							chip.SetKeys(0x5, 0);
							break;

						case SDLK_e:
							chip.SetKeys(0x6, 0);
							break;

						case SDLK_a:
							chip.SetKeys(0x7, 0);
							break;

						case SDLK_s:
							chip.SetKeys(0x8, 0);
							break;

						case SDLK_d:
							chip.SetKeys(0x9, 0);
							break;

						case SDLK_z:
							chip.SetKeys(0xA, 0);
							break;

						case SDLK_c:
							chip.SetKeys(0xB, 0);
							break;

						case SDLK_4:
							chip.SetKeys(0xC, 0);
							break;

						case SDLK_r:
							chip.SetKeys(0xD, 0);
							break;

						case SDLK_f:
							chip.SetKeys(0xE, 0);
							break;

						case SDLK_v:
							chip.SetKeys(0xF, 0);
							break;
					}
		    }
    }
}

bool Graphics::Needs_Stop()
{
    return should_stop;
}