/**
 * hello2_window.c - Initializes SDL and creates a window
 */

#include <fstream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "../include/MazeUtils.h"

int main(void)
{
    /*
    // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Pac that man!",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       640, 480, 0);
    if (!win)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
	    return 1;
    }

    // wait a few seconds
    SDL_Delay(5000);

    // clean up resources before exiting
    SDL_DestroyWindow(win);
    SDL_Quit();
    */

    std::string filePath = "../res/maze1.txt";
    pacman::mazeutils::readMazeMatrix(filePath);

    return 0;
}