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
class Game : public std::enable_shared_from_this<Game> {
public:
  // Some hard-coded cell-width and cell-heights.
  static const long CELL_WIDTH;
  static const long CELL_HEIGHT;

  // Constants used for parsing agents config file.
  static const std::string PACMAN;
  static const std::string GHOST;
  static const char DELIM;

  /** A static factory method to create a game. This replaces the role of the
   * constructor. This allows the use of shared_from_this. */
  template <typename... T>
  static std::shared_ptr<pacman::Game> create(T &&... t) {
    auto shared_ptr = std::shared_ptr<Game>(new Game(std::forward<T>(t)...));
    shared_ptr->initialize();
    return shared_ptr;
  }

  /** Start the game. */
  void start();

  /** Stop the game with a message. */
  void stop(std::string message);

private:
  // Constructor is private. Must use factory method to create.
  Game(std::string gameConfigFolderPath, std::string imgFolderPath);
  void initialize();

  /** Whether the game is running. The game-loop is conditioned on this. */
  std::atomic_bool running;

  /** Points to a directory with the agents and maze config files. */
  std::string gameConfigFolderPath;

  /** Img Folder path has resources to render the game. */
  std::string imgFolderPath;

  // The major components of the game that Game owns.
  std::shared_ptr<Maze> maze;
  std::shared_ptr<Pacman> pacman;
  std::vector<std::shared_ptr<Ghost>> ghosts;
  std::unique_ptr<SDLWrapper> sdlWrapper;

  // Initialization of above major components.
  void initMaze(std::string mazeConfigPath);
  void initAgents(std::string agentsConfigPath);
  void initUI(std::string imgFolderPath);

  // Some helper methods to parse config.
  std::vector<std::pair<int, int>>
  parseIndicesByPrefix(std::string agentsConfigPath, std::string agent);
  long indexToLocation(int index, long scale);

  /** Helper to get vector of weak_ptrs from vector of shared_ptrs, to share
   * reference of ghosts. */
  std::vector<std::weak_ptr<Ghost>> getGhostsWeakPointers();
};

} // namespace pacman

#endif // PACMAN_GAME_H