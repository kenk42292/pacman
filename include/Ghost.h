#ifndef PACMAN_GHOST_H_
#define PACMAN_GHOST_H_

#include <functional>

#include "../include/Agent.h"

namespace pacman {

class Pacman;

class Ghost : public Agent {

public:
  static const long GHOST_VELOCITY =
      3; // same as pacman. However, ghosts are slower anyway, because of A*
         // search latency.
  Ghost(long y, long x, std::shared_ptr<Maze> maze,
        std::shared_ptr<Pacman> pacman);
  void start();
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
  static long manhattan_distance(std::pair<int, int> startCell,
                          std::pair<int, int> endCell);
  std::shared_ptr<Pacman> pacman;
};
} // namespace pacman

#endif // PACMAN_GHOST_H_
