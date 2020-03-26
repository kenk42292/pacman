#ifndef PACMAN_AGENT_H_
#define PACMAN_AGENT_H_

namespace pacman {

class Agent {
public:
    Agent(long y, long x, long velocity);
    long getY() const;
    long getX() const;
    long getVelocity() const;
private:
    long y; // y-value of the CENTER of the agent
    long x; // x-value of the CENTER of the agent
    long velocity;
};

} // pacman

#endif // PACMAN_AGENT_H_
