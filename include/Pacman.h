#ifndef PACMAN_PACMAN_H_
#define PACMAN_PACMAN_H_

#include <atomic>
#include <iostream>
#include <memory>

#include "Agent.h"
#include "ui/KeyEventListener.h"

namespace pacman {

class Maze;
namespace ui {
  class KeyEventListener;
}

class Pacman : public ui::KeyEventListener, public Agent {
public:
  enum Orientation { UP, DOWN, LEFT, RIGHT };
  enum MouthDirection {OPENING, CLOSING};

  static const long PACMAN_VELOCITY = 3;
  Pacman(long y, long x, std::shared_ptr<Maze> maze);
  void start();
  long getMouthDegrees();
  Orientation getOrientation();
  void onKeyEvent(int scanCode) override;
  friend std::ostream &operator<<(std::ostream &outputStream,
                                  const Pacman &pacman);

private:
  Orientation orientation;
  Orientation desiredOrientation;
  std::atomic_long mouthDegrees;
  MouthDirection mouthDirection;
  std::pair<long, long> nextLocation(Orientation orientation);
  std::pair<long, long> nextCellLocation(Orientation orientation);
};

} // namespace pacman

#endif // PACMAN_PACMAN_H_