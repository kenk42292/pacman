#include <algorithm>
#include <fstream>
#include <future>
#include <iostream>
#include <string>
#include <utility>

#include "../include/Game.h"
#include "../include/Ghost.h"
#include "../include/Pacman.h"
#include "../include/SDLWrapper.h"

pacman::Game::Game(std::string gameConfigFolderPath, std::string imgFolderPath)
    : gameConfigFolderPath(gameConfigFolderPath), imgFolderPath(imgFolderPath) {}

void pacman::Game::initialize() {
  initMaze(gameConfigFolderPath + "/maze");
  initAgents(gameConfigFolderPath + "/agents");
  maze->injectAgents(pacman, getGhostsWeakPointers());
  initUI(imgFolderPath);
}

void pacman::Game::start() {

  // Game Logic in the background.
  auto pacmanFuture =
      std::async(std::launch::async, [this]() { pacman->start(); });

  std::vector<std::future<void>> ghostFutures;
  for (auto ghost_iter = ghosts.begin(); ghost_iter < ghosts.end();
       ghost_iter++) {
    ghostFutures.push_back(std::async(
        std::launch::async, [ghost_iter]() { (*ghost_iter)->start(); }));
  }

  // Start Game Loop.
  running = true;
  while (running) {
    sdlWrapper->processInputEvents();
    sdlWrapper->render();
  }
}

void pacman::Game::stop(std::string message) {
  std::cout << message << "\n";

  running = false;

  pacman->stop();

  for (auto ghost_iter = ghosts.begin(); ghost_iter < ghosts.end();
       ghost_iter++) {
    (*ghost_iter)->stop();
  }
}

void pacman::Game::initMaze(std::string mazeConfigPath) {
  maze = std::make_shared<Maze>(mazeConfigPath, CELL_HEIGHT, CELL_WIDTH);
}

void pacman::Game::initAgents(std::string agentsConfigPath) {
  long y, x;

  auto pacmanIndicesVector = parseIndicesByPrefix(agentsConfigPath, PACMAN);
  if (pacmanIndicesVector.size() == 0) {
    throw std::invalid_argument("No pacman location found.");
  }
  auto pacmanIndeces = pacmanIndicesVector.at(0);
  y = indexToLocation(pacmanIndeces.first, CELL_HEIGHT);
  x = indexToLocation(pacmanIndeces.second, CELL_WIDTH);
  pacman = std::make_shared<Pacman>(y, x, maze, shared_from_this());

  auto ghostIndicesVector = parseIndicesByPrefix(agentsConfigPath, GHOST);
  for (auto ghostIndices : ghostIndicesVector) {
    y = indexToLocation(ghostIndices.first, CELL_HEIGHT);
    x = indexToLocation(ghostIndices.second, CELL_WIDTH);
    ghosts.push_back(std::make_shared<Ghost>(y, x, maze, pacman, shared_from_this()));
  }
}

void pacman::Game::initUI(std::string imgFolderPath) {
  sdlWrapper = std::make_unique<SDLWrapper>(
      maze, pacman, getGhostsWeakPointers(), imgFolderPath, shared_from_this());
}

std::vector<std::pair<int, int>>
pacman::Game::parseIndicesByPrefix(std::string agentsConfigPath,
                                   std::string agent) {
  if (agent != PACMAN && agent != GHOST) {
    throw std::invalid_argument("Agent must be either pacman or ghost.");
  }

  std::ifstream agentsConfigFileStream(agentsConfigPath);
  if (!agentsConfigFileStream.is_open()) {
    std::cout << "Failed to read in input file[" << agentsConfigPath << "]"
              << '\n';
    exit(1);
  }

  std::vector<std::pair<int, int>> result;
  std::string line;
  while (std::getline(agentsConfigFileStream, line)) {
    // remove spaces
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

    if (line.rfind(agent, 0) == 0) {
      std::string locStr = line.substr(agent.length() + 1);
      int delimIndex = locStr.find(DELIM);
      int i = stol(locStr.substr(0, delimIndex));
      int j = stol(locStr.substr(delimIndex + 1));
      result.push_back(std::make_pair(i, j));
    }
  }

  return result;
}

long pacman::Game::indexToLocation(int index, long scale) {
  return index * scale + scale / 2;
}

std::vector<std::weak_ptr<pacman::Ghost>>
pacman::Game::getGhostsWeakPointers() {
  std::vector<std::weak_ptr<Ghost>> weakPointers;
  for (auto ghost_iter = ghosts.begin(); ghost_iter < ghosts.end();
       ghost_iter++) {
    weakPointers.push_back((*ghost_iter));
  }
  return weakPointers;
}
