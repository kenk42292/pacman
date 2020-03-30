
#include <iostream>

#include "../include/Ghost.h"

pacman::Ghost::Ghost(long y, long x, std::shared_ptr<Maze> maze) : Agent(y, x, GHOST_VELOCITY, maze) {}

namespace pacman {
/** Make ghost printable via std::cout. */
std::ostream &operator<<(std::ostream &outputStream,
                         const pacman::Ghost &ghost) {
  outputStream << "ghost{loc[" << ghost.y << ", " << ghost.x
               << "] vel[" << ghost.velocity << "]}";
}
} // namespace pacman
