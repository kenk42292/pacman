
#include "../include/Agent.h"
#include "../include/Maze.h"

pacman::Agent::Agent(long y, long x, long velocity, std::shared_ptr<Maze> maze) : y(y), x(x), velocity(velocity), maze(maze), alive(false) {}

void pacman::Agent::start() {
    alive.store(true);
}

void pacman::Agent::stop() {
    alive.store(false);
}
