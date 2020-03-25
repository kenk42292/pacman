
#include <iostream>

#include "../include/Ghost.h"

pacman::Ghost::Ghost(long y, long x) : Agent(y, x, GHOST_VELOCITY) {}

namespace pacman {
/** Make ghost printable via std::cout. */
std::ostream &operator<<(std::ostream &outputStream,
                         const pacman::Ghost &ghost) {
  outputStream << "ghost{loc[" << ghost.getY() << ", " << ghost.getX()
               << "] vel[" << ghost.getVelocity() << "]}";
}
} // namespace pacman
