#include <string>

#include "../include/Game.h"
#include "../include/ui/SDLWrapper.h"

pacman::Game::Game(std::string gameConfigFolderPath, std::string imgFolderPath) {
    initMaze(gameConfigFolderPath + "/maze");
    initAgents(gameConfigFolderPath + "/agents");
    // maze->
    initUI(imgFolderPath);
}

void pacman::Game::start() {
    sdlWrapper->start();
}

void pacman::Game::initMaze(std::string mazeConfigPath) {
    maze = std::make_shared<Maze>(mazeConfigPath, CELL_HEIGHT, CELL_WIDTH);
}

void pacman::Game::initAgents(std::string agentConfigPath) {

}

void pacman::Game::initUI(std::string imgFolderPath) {
    // Start User interaction with components of the game.
    // pacman::ui::SDLWrapper sdlWrapper(game, maze, pacman, ghosts);
    sdlWrapper = std::make_unique<ui::SDLWrapper>(maze, pacman, ghosts, imgFolderPath);
}

