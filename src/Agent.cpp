
#include "../include/Agent.h"

pacman::Agent::Agent(long y, long x, long velocity) : y(y), x(x), velocity(velocity) {}

long pacman::Agent::getY() const {
    return y;
}

long pacman::Agent::getX() const {
    return x;
}

long pacman::Agent::getVelocity() const {
    return velocity;
}
