#ifndef PACMAN_MAZE_H_
#define PACMAN_MAZE_H_

#include <memory>
#include <vector>

namespace pacman {

class Ghost;
class Pacman;

class Maze {
public:
  enum Cell { kEmpty, kPellet, kBorder };
  
  Maze(std::shared_ptr<std::vector<std::vector<Cell>>> mazeMatrix,
       long cellHeight, long cellWidth);
  void init();
  std::shared_ptr<std::vector<std::vector<Cell>>> getMazeMatrix();
  bool isValid(std::pair<long, long> location);
  bool isCellAligned(std::pair<long, long> location);
  long distance(std::pair<long, long> a, std::pair<long, long> b);
  // std::shared_ptr<Pacman> getPacman();
  // std::vector<std::shared_ptr<Ghost>> getGhosts();
  // void injectAgents(Pacman pacman, std::vector<std::shared_ptr<Ghost>> ghosts);
  long getCellHeight();
  long getCellWidth();

private:
  std::shared_ptr<std::vector<std::vector<Cell>>> mazeMatrix;
  // std::shared_ptr<Pacman> pacman;
  // std::vector<std::shared_ptr<Ghost>> ghosts;
  long cellHeight;
  long cellWidth;
};

} // namespace pacman

#endif // PACMAN_MAZE_H_
