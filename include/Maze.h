
#include <vector>

#include "Cell.h"
#include "Ghost.h"
#include "Pacman.h"

#ifndef PACMAN_MAZE_H_
#define PACMAN_MAZE_H_

namespace pacman {

class Maze {
public:
    void init();
    void addListener();

private:
    std::vector<std::vector<Cell>> mazeMatrix;
    Pacman pacman;
    std::vector<Ghost> ghosts;
};

} // pacman

#endif // PACMAN_MAZE_H_

