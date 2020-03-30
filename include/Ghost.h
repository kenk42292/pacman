#ifndef PACMAN_GHOST_H_
#define PACMAN_GHOST_H_

#include "../include/Agent.h"

namespace pacman {

class Ghost : public Agent {
public:
    static const long GHOST_VELOCITY = 45;
    Ghost(long y, long x, std::shared_ptr<Maze> maze);
private:
};

std::ostream& operator<<(std::ostream &outputStream, const pacman::Ghost &ghost);

} // pacman

#endif // PACMAN_GHOST_H_
