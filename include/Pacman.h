#ifndef PACMAN_PACMAN_H_
#define PACMAN_PACMAN_H_

#include "Agent.h"
#include "Orientation.h"
#include "ui/KeyEventListener.h"

#include <iostream>

namespace pacman {

class Pacman : public KeyEventListener, public Agent {
public:
  static const long PACMAN_VELOCITY = 60;
  Pacman(long y, long x);
  void onKeyEvent(int scanCode) override;
  friend std::ostream &operator<<(std::ostream &outputStream,
                                  const Pacman &pacman);

private:
  Orientation orientation;
};

} // namespace pacman

#endif // PACMAN_PACMAN_H_