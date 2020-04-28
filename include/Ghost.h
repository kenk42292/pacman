#ifndef PACMAN_GHOST_H_
#define PACMAN_GHOST_H_

#include <functional>

#include "../include/Agent.h"

namespace pacman {

class Game;
class Pacman;

class Ghost : public Agent {

public:
  static const long GHOST_VELOCITY;
  static const long GHOST_DELAY_MILLIS;

  Ghost(long y, long x, std::weak_ptr<Maze> maze,
        std::weak_ptr<Pacman> pacman_weak_ptr,
        std::weak_ptr<Game> game_weak_ptr);
  void start() override;
  friend std::ostream &operator<<(std::ostream &outputStream,
                                  const pacman::Ghost &ghost);

private:
  /** Node used for a-star search. */
  struct Node {
    int i;
    int j;
    Node *parent;
    long g_val; // distance so far
    long f_val; // g_val + heuristic (manhattan distance)
    Node(int i, int j) : i(i), j(j) {}
  };

  /** Custom comparator for min. priority queue of Nodes. */
  struct NodeComparator {
    // Since priority queues are default max. priority, inverted value comparison.
    bool operator()(const Node* a, const Node* b) { return a->f_val > b->f_val; }
  };

  /** Returns the coordinates of the cell to go to next, if travelling from
   * start to end. */
  std::pair<int, int> astar(std::pair<int, int> startCell,
                            std::pair<int, int> endCell,
                            std::function<long(std::pair<int, int> startCell,
                                               std::pair<int, int> endCell)>
                                heuristic);

  /** A simple heuristic for A*. May come up with others at some point. */
  static long manhattan_distance(std::pair<int, int> startCell,
                          std::pair<int, int> endCell);

  /** Reference to pacman.  */
  std::weak_ptr<Pacman> pacman_weak_ptr;
  
  /** Reference to the game. When this ghost captures Pacman, stop the game. */
  std::weak_ptr<Game> game_weak_ptr;
};
} // namespace pacman

#endif // PACMAN_GHOST_H_
