#ifndef PACMAN_PACMAN_H_
#define PACMAN_PACMAN_H_

#include <atomic>
#include <iostream>
#include <memory>

#include "Agent.h"

namespace pacman {

class Game;
class Maze;

class Pacman : public Agent {
public:
  enum Orientation { UP, DOWN, LEFT, RIGHT };
  enum MouthDirection {OPENING, CLOSING};

  static const long PACMAN_VELOCITY = 3;
  Pacman(long y, long x, std::weak_ptr<Maze> maze_weak_ptr, std::weak_ptr<Game> game_weak_ptr, int goalNumPellets);
  void start();
  long getMouthDegrees();
  Orientation getOrientation();
  void onKeyEvent(int scanCode);
  friend std::ostream &operator<<(std::ostream &outputStream,
                                  const Pacman &pacman);

private:
  std::weak_ptr<Game> game_weak_ptr;
  Orientation orientation;
  Orientation desiredOrientation;
  std::atomic_long mouthDegrees;
  MouthDirection mouthDirection;
  std::pair<long, long> nextLocation(Orientation orientation);
  std::pair<long, long> nextCellLocation(Orientation orientation);
  std::atomic_long numPelletsEaten;
  int goalNumPellets;
};

} // namespace pacman

#endif // PACMAN_PACMAN_H_