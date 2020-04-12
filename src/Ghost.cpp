
#include <bits/stdc++.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "../include/Ghost.h"
#include "../include/Maze.h"
#include "../include/Pacman.h"

pacman::Ghost::Ghost(long y, long x, std::shared_ptr<Maze> maze,
                     std::shared_ptr<Pacman> pacman)
    : Agent(y, x, GHOST_VELOCITY, maze), pacman(pacman) {}

void pacman::Ghost::start() {
  Agent::start();
  while (alive.load()) {
    std::cout << *this << '\n';

    // Delay
    std::this_thread::sleep_for(std::chrono::milliseconds(30));

    // Calculate which cell to go to next
    long cellHeight = maze->getCellHeight(), cellWidth = maze->getCellWidth();
    std::pair<int, int> currentCellPosition(getY() / cellHeight,
                                            getX() / cellWidth);
    std::pair<int, int> pacmanCellPosition(pacman->getY() / cellHeight,
                                           pacman->getX() / cellWidth);
    std::pair<int, int> nextCellPosition = astar(
        currentCellPosition, pacmanCellPosition, Ghost::manhattan_distance);

    // Move
    int delta_i = nextCellPosition.first - currentCellPosition.first;
    int delta_j = nextCellPosition.second - currentCellPosition.second;
    // <delta_i, delta_j> should be a one-hot pair.
    long delta_y = delta_i * velocity;
    long delta_x = delta_j * velocity;
    long y_final = getY() + delta_y;
    long x_final = getX() + delta_x;
    std::pair<long, long> loc_final = std::make_pair(y_final, x_final);
    if (maze->isAxisAligned(loc_final))
    // If the move resulted in an alignment with column or row, the move is
    // valid: move that way.
    {
      this->y.store(y_final);
      this->x.store(x_final);
    } else
    // If not axis-aligned, must align to axis first. Move towards center of
    // current cell. Eventually, we'll be cell-aligned. From there, we'll move
    // while maintaining axis-alignment.
    {
      // Center of current cell
      y_final = currentCellPosition.first * cellHeight + cellHeight / 2;
      x_final = currentCellPosition.second * cellWidth + cellWidth / 2;

      // Move towards center of cell.
      delta_y = y_final - y;
      if (std::abs(delta_y) > velocity) {
        delta_y = delta_y < 0 ? -velocity : velocity;
      }
      delta_x = x_final - x;
      if (std::abs(delta_x) > velocity) {
        delta_x = delta_x < 0 ? -velocity : velocity;
      }

      this->y.store(getY() + delta_y);
      this->x.store(getX() + delta_x);
    }
  }
}

std::pair<int, int>
pacman::Ghost::astar(std::pair<int, int> startCell, std::pair<int, int> endCell,
                     std::function<long(std::pair<int, int> startCell,
                                        std::pair<int, int> endCell)>
                         heuristic) {

  int numRows = maze->getMazeMatrix()->size();
  int numCols = maze->getMazeMatrix()->at(0).size();

  // Pre-create all nodes.
  std::vector<std::vector<Node>> nodes;
  for (int i = 0; i < numRows; i++) {
    std::vector<Node> row;
    for (int j = 0; j < numCols; j++) {
      Node node(i, j);
      node.g_val = LONG_MAX;
      node.f_val = LONG_MAX;
      node.parent = nullptr;
      row.push_back(node);
    }
    nodes.push_back(row);
  }

  // Set of closed nodes. The nodes are tracked as their indices, determined as
  // 'index = i*numCols + j
  std::unordered_set<int> closed;

  std::priority_queue<Node *, std::vector<Node *>, NodeComparator> open;

  Node *startNode = &nodes.at(startCell.first).at(startCell.second);
  startNode->g_val = 0;
  startNode->f_val = heuristic(startCell, endCell);
  startNode->parent = startNode;
  open.push(startNode);
  while (open.size() > 0) {
    // Grab min. f_val node from heap.
    Node *current = open.top();
    open.pop();
    closed.insert(current->i * numCols + current->j);

    // If we've reached the goal, find the next node that the ghost should go
    // to, via parent pointers.
    if (current->i == endCell.first && current->j == endCell.second) {
      while (current->parent->i != startCell.first ||
             current->parent->j != startCell.second) {
        current = current->parent;
      }
      return std::make_pair(current->i, current->j);
    }

    // Iterate through neighbors.
    int i = current->i, j = current->j;
    std::vector<std::pair<int, int>> candidates{
        std::make_pair(i - 1, j), std::make_pair(i + 1, j),
        std::make_pair(i, j - 1), std::make_pair(i, j + 1)};
    for (auto candidate = candidates.begin(); candidate < candidates.end();
         candidate++) {

      // If candidate is already closed, don't consider it.
      if (closed.find(candidate->first * numCols + candidate->second) !=
          closed.end()) {
        continue;
      }

      // If candidate isn't possible in possible maze, don't consider it.
      // Further, if the planned location overlaps with any agent besides
      // itself, don't consider it.
      if (!maze->isCellValid(*candidate) ||
          maze->isGhostInCell(*candidate)) {
        continue;
      }

      // Add neighbor to open set.
      Node *neighbor = &nodes.at(candidate->first).at(candidate->second);
      if (neighbor->g_val > current->g_val + 1) {
        neighbor->g_val = current->g_val + 1;
        neighbor->f_val =
            neighbor->g_val +
            heuristic(std::make_pair(candidate->first, candidate->second),
                      std::make_pair(endCell.first, endCell.second));
        neighbor->parent = current;
        open.push(neighbor);
      }
    }
  }
  // Failed. Log and return startCell, to remain in starting cell.
  std::cout << "Failed to find next node to traverse to.\n";
  return startCell;
}

long pacman::Ghost::manhattan_distance(std::pair<int, int> startCell,
                                       std::pair<int, int> endCell) {
  return std::abs(endCell.first - startCell.first) +
         std::abs(endCell.second - startCell.second);
}

namespace pacman {
/** Make ghost printable via std::cout. */
std::ostream &operator<<(std::ostream &outputStream,
                         const pacman::Ghost &ghost) {
  outputStream << "ghost{loc[" << ghost.y << ", " << ghost.x << "] vel["
               << ghost.velocity << "]}";
}
} // namespace pacman
