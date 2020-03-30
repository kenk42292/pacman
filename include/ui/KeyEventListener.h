#ifndef PACMAN_UI_KEYEVENTLISTENER_H_
#define PACMAN_UI_KEYEVENTLISTENER_H_

namespace pacman {
namespace ui {

class KeyEventListener {
public:
  virtual void onKeyEvent(int scanCode) = 0;
};

} // namespace ui
} // namespace pacman

#endif // PACMAN_UI_KEYEVENTLISTENER_H_
