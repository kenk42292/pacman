#ifndef PACMAN_UI_KEYEVENTLISTENER_H_
#define PACMAN_UI_KEYEVENTLISTENER_H_

class KeyEventListener {
public:
    virtual void onKeyEvent(int scanCode) = 0;
};

#endif // PACMAN_UI_KEYEVENTLISTENER_H_
