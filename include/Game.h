#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H

#include <string>
#include <memory>
#include <vector>

#include "Maze.h"
#include "ui/SDLWrapper.h"

namespace pacman {

class Pacman;
class Ghost;

/**
 * Responsible for parsing inputs, and starting the game by standing up all the 
 * components of the game: Agents and UI.
 */
class Game {
public:
    Game(std::string gameConfigFolderPath, std::string imgFolderPath);
    const long CELL_WIDTH = 20;
    const long CELL_HEIGHT = 20;
    const std::string PACMAN = "pacman";
    const std::string GHOST = "ghost";
    const char DELIM = ',';
    void start();
private:
    std::shared_ptr<Maze> maze;
    std::shared_ptr<Pacman> pacman;
    std::vector<std::shared_ptr<Ghost>> ghosts;
    std::unique_ptr<ui::SDLWrapper> sdlWrapper;
    void initMaze(std::string mazeConfigPath);
    void initAgents(std::string agentsConfigPath);
    void initUI(std::string imgFolderPath);
    std::shared_ptr<std::vector<std::vector<Maze::Cell>>> readMazeMatrix(std::string mazeFilePath);
    Maze::Cell charToCell(char & c);
    std::vector<std::pair<int, int>> parseIndicesByPrefix(std::string agentsConfigPath, std::string agent);
    long indexToLocation(int index, long scale);
};

}

#endif // PACMAN_GAME_H