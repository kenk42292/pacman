#ifndef PACMAN_PACMAN_H_
#define PACMAN_PACMAN_H_

#include "ui/KeyEventListener.h"

namespace pacman {

class Pacman : public KeyEventListener {
    void onKeyEvent(int scanCode) override;
};

} // pacman

#endif // PACMAN_PACMAN_H_