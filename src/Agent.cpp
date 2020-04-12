
#include "../include/Agent.h"
#include "../include/Maze.h"

pacman::Agent::Agent(long y, long x, long velocity, std::shared_ptr<Maze> maze) : y(y), x(x), velocity(velocity), maze(maze), alive(false) {}

void pacman::Agent::start() {
    alive.store(true);
}

void pacman::Agent::stop() {
    alive.store(false);
}

long pacman::Agent::getY() const {
    return y.load();
}

long pacman::Agent::getX() const {
    return x.load();
}

long pacman::Agent::getVelocity() const {
    return velocity;
}