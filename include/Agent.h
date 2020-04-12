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
    long getY() const;
    long getX() const;
    long getVelocity() const;
    std::atomic_bool alive;
protected:
    std::atomic_long y;
    std::atomic_long x;
    long velocity;
    std::shared_ptr<Maze> maze;
};

} // pacman

#endif // PACMAN_AGENT_H_
