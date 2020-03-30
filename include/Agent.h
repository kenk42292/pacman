#ifndef PACMAN_AGENT_H_
#define PACMAN_AGENT_H_

#include <atomic>
#include <memory>

namespace pacman {

class Maze;

class Agent {
public:
    Agent(long y, long x, long velocity, std::shared_ptr<Maze>);
    void start();
    void stop();
    std::atomic_bool alive;
    long y; // y-value of the CENTER of the agent
    long x; // x-value of the CENTER of the agent
    long velocity;
    std::shared_ptr<Maze> maze;
};

} // pacman

#endif // PACMAN_AGENT_H_
