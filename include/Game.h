#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H

#include <string>
#include <vector>

#include "../include/Maze.h"
#include "../include/ui/SDLWrapper.h"

namespace pacman {

class Game {
public:
    Game(std::string gameConfigFolderPath, std::string imgFolderPath);
    const long CELL_WIDTH = 30;
    const long CELL_HEIGHT = 30;
    void start();
private:
    std::shared_ptr<Maze> maze;
    std::shared_ptr<Pacman> pacman;
    std::vector<std::shared_ptr<Ghost>> ghosts;
    std::unique_ptr<ui::SDLWrapper> sdlWrapper;
    void initMaze(std::string mazeConfigPath);
    void initAgents(std::string agentsConfigPath);
    void initUI(std::string imgFolderPath);
};

}

#endif // PACMAN_GAME_H