#ifndef PACMAN_MAZE_H_
#define PACMAN_MAZE_H_

#include <memory>
#include <vector>

namespace pacman {

class Ghost;
class Pacman;

class Maze {
public:
  /** Internal representation of maze is as 2-D vector of following cells. */
  enum Cell { kEmpty, kPellet, kBorder };
  
  /** Creates a maze with internal maze matrix representation, with specified cell dimensions. */
  Maze(std::string mazeFilePathx, long cellHeight, long cellWidth);

  /** Gives agent references to maze. */
  void injectAgents(std::weak_ptr<Pacman> pacman, std::vector<std::weak_ptr<Ghost>> ghosts);

  /** Returns whether the given location (in pixels) is a valid place for an agent to be centered on. */
  bool isValid(std::pair<long, long> location);

  /** Returns whether the given cell coordinate is a valid cell for an agent to be in. */
  bool isCellValid(std::pair<int, int> coordinates);

  /** Returns whether the given location (in pixels) is aligned(at the center of) a cell. */
  bool isCellAligned(std::pair<long, long> location);

  /** Returns whether the given location (in pixels) is aligned to a row or column. */
  bool isAxisAligned(std::pair<long, long> location);

  long distance(std::pair<long, long> a, std::pair<long, long> b);

  /** Removes a pellet from the maze, if the agent overlaps with it. */
  bool clearPellet(std::pair<long, long> agentCenter);

  /** Returns the number of ghosts that overlap with a location. */
  bool isGhostInCell(std::pair<long, long> location);

  /** Reads the maze matrix from the given file. */
  std::shared_ptr<std::vector<std::vector<Maze::Cell>>> readMazeMatrix(std::string mazeFilePath);

  /** Utility for reading maze matrix. */
  Maze::Cell charToCell(char & c);

  // Getters and Setters.
  std::weak_ptr<Pacman> getPacman();
  std::vector<std::weak_ptr<Ghost>> getGhosts();
  long getCellHeight();
  long getCellWidth();
  std::weak_ptr<std::vector<std::vector<Cell>>> getMazeMatrix();
  
private:
  std::shared_ptr<std::vector<std::vector<Cell>>> mazeMatrix;
  std::weak_ptr<Pacman> pacman;
  std::vector<std::weak_ptr<Ghost>> ghosts;
  long cellHeight;
  long cellWidth;
};

} // namespace pacman

#endif // PACMAN_MAZE_H_
