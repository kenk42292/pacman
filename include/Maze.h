
#include <vector>

#include "Cell.h"
#include "Ghost.h"
#include "Pacman.h"

#ifndef PACMAN_MAZE_H_
#define PACMAN_MAZE_H_

#include <memory>

namespace pacman {

class Maze {
public:
    Maze(std::string mazeConfigPath, long cellHeight, long cellWidth);
    ~Maze();
    void init();
    std::shared_ptr<std::vector<std::vector<Cell>>> getMazeMatrix();
    std::shared_ptr<Pacman> getPacman();
    std::vector<std::shared_ptr<Ghost>> getGhosts();
    void injectAgents(Pacman pacman, std::vector<std::shared_ptr<Ghost>> ghosts);
    long getCellHeight();
    long getCellWidth();
private:
    std::shared_ptr<std::vector<std::vector<Cell>>> mazeMatrix;
    std::shared_ptr<Pacman> pacman;
    std::vector<std::shared_ptr<Ghost>> ghosts;
    std::shared_ptr<std::vector<std::vector<Cell>>> readMazeMatrix(std::string mazeFilePath);
    Cell charToCell(char & c);
    long cellHeight;
    long cellWidth;
};

} // pacman

#endif // PACMAN_MAZE_H_

