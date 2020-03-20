#ifndef PACMAN_MAZEUTILS_H
#define PACMAN_MAZEUTILS_H

#include <fstream>
#include <vector>

#include "Cell.h"

namespace pacman {
namespace mazeutils {

    std::vector<std::vector<Cell>> readMazeMatrix(std::string mazeFilePath);

    Cell charToCell(char & c);

} // mazeutils
} // pacman

#endif // PACMAN_MAZEUTILS_H