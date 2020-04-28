#ifndef PACMAN_AGENT_H_
#define PACMAN_AGENT_H_

#include <atomic>
#include <memory>

namespace pacman {

class Maze;

/** Agent that lives on a given maze. Its lifecycle can be controlled via #start
 * and #stop methods. */
class Agent {
public:
  Agent(long y, long x, long velocity, std::weak_ptr<Maze> maze_weak_ptr);
  virtual void start();
  void stop();
  long getY() const;
  long getX() const;
  long getVelocity() const;
  std::atomic_bool alive;

protected:
  std::atomic_long y;
  std::atomic_long x;
  long velocity;
  std::weak_ptr<Maze> maze_weak_ptr;
};

} // namespace pacman

#endif // PACMAN_AGENT_H_
