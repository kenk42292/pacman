#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H

#include <memory>
#include <string>
#include <vector>

#include "Maze.h"
#include "SDLWrapper.h"

namespace pacman {

class Pacman;
class Ghost;

/**
 * Responsible for parsing inputs, and starting the game by standing up all the
 * components of the game: Maze, Agents and UI. Game also owns those three major
 * components shared_ptr. Other components should reference each other via
 * weak_ptr, if necessary.
 */
class Game {
public:
  Game(std::string gameConfigFolderPath, std::string imgFolderPath);

  // Some hard-coded cell-width and cell-heights.
  const long CELL_WIDTH = 20;
  const long CELL_HEIGHT = 20;

  // Constants used for parsing agents config file.
  const std::string PACMAN = "pacman";
  const std::string GHOST = "ghost";
  const char DELIM = ',';

  /** Start the game. */
  void start();

  /** Stop the game with a message. */
  void stop(std::string message);

private:
  std::shared_ptr<Maze> maze;
  std::shared_ptr<Pacman> pacman;
  std::shared_ptr<std::vector<Ghost*>> ghosts;
  std::unique_ptr<SDLWrapper> sdlWrapper;
  void initMaze(std::string mazeConfigPath);
  void initAgents(std::string agentsConfigPath);
  void initUI(std::string imgFolderPath);
  std::vector<std::pair<int, int>>
  parseIndicesByPrefix(std::string agentsConfigPath, std::string agent);
  long indexToLocation(int index, long scale);
};

} // namespace pacman

#endif // PACMAN_GAME_H