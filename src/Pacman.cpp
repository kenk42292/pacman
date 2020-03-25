
#include <SDL2/SDL.h>
#include <iostream>

#include "../include/Orientation.h"
#include "../include/Pacman.h"

pacman::Pacman::Pacman(long y, long x)
    : Agent(y, x, PACMAN_VELOCITY), orientation(UP) {}

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

/** Make pacman printable via std::cout. */
namespace pacman {
std::ostream &operator<<(std::ostream &outputStream,
                         const pacman::Pacman &pacman) {
  outputStream << "pacman{loc[" << pacman.getY() << ", " << pacman.getX()
               << "] vel[" << pacman.getVelocity() << "] orientation:["
               << pacman.orientation << "]}";
}
} // namespace pacman