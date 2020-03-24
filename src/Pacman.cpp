
#include <iostream>
#include <SDL2/SDL.h>

#include "../include/Pacman.h"

void pacman::Pacman::onKeyEvent(int scanCode) {
    switch (scanCode) {
        case SDL_SCANCODE_LEFT:
            std::cout << "LEFT" << '\n';
            break;
        case SDL_SCANCODE_RIGHT:
            std::cout << "RIGHT" << '\n';
            break;
        case SDL_SCANCODE_UP:
            std::cout << "UP" << '\n';
            break;
        case SDL_SCANCODE_DOWN:
            std::cout << "DOWN" << '\n';
            break;
        default:
            std::cout << "no-op" << '\n';
    }
}
