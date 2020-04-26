
#include <SDL2/SDL.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "../include/Maze.h"
#include "../include/Pacman.h"

pacman::Pacman::Pacman(long y, long x, std::weak_ptr<Maze> maze)
    : Agent(y, x, PACMAN_VELOCITY, maze), orientation(UP),
      desiredOrientation(UP), mouthDegrees(0), mouthDirection(OPENING) {}

void pacman::Pacman::start() {
  Agent::start();
  while (alive.load()) {

    // Delay
    std::this_thread::sleep_for(std::chrono::milliseconds(30));

    // Move mouth
    switch (mouthDirection) {
    case OPENING:
      mouthDegrees += 10;
      if (mouthDegrees > 120) {
        mouthDirection = CLOSING;
      }
      break;
    case CLOSING:
      mouthDegrees -= 10;
      if (mouthDegrees < 0) {
        mouthDirection = OPENING;
      }
      break;
    }

    auto maze = maze_weak_ptr.lock();

    // Eat Pellet
    std::pair<long, long> currentLocation = std::make_pair(getY(), getX());
    bool pelletEaten = maze->clearPellet(currentLocation);
    if (pelletEaten) {
      numPelletsEaten++;
    }

    // Move
    std::pair<long, long> nextDesiredLocation =
        this->nextLocation(desiredOrientation);
    std::pair<long, long> nextLocation = this->nextLocation(orientation);
    std::pair<long, long> nextCellLocation =
        this->nextCellLocation(orientation);
    if (orientation == desiredOrientation) {
      if (maze->isValid(nextDesiredLocation))
      // If pacman can move straight to next desired location, do so.
      {
        y.store(nextLocation.first);
        x.store(nextLocation.second);
      } else if (maze->isValid(nextCellLocation))
      // Otherwise, move until flush with cell we're currently entering.
      {
        y.store(nextCellLocation.first);
        x.store(nextCellLocation.second);
      }
    } else {
      if (maze->isCellAligned(currentLocation)) {
        if (maze->isValid(nextDesiredLocation)) {
          orientation = desiredOrientation;
          y.store(nextDesiredLocation.first);
          x.store(nextDesiredLocation.second);
        } else if (maze->isValid(nextLocation)) {
          y.store(nextLocation.first);
          x.store(nextLocation.second);
        } else if (maze->isValid(nextCellLocation)) {
          y.store(nextCellLocation.first);
          x.store(nextCellLocation.second);
        }
      } else {
        // TODO: Refine, if it jitters when stopping at cells
        if (maze->distance(currentLocation, nextLocation) <
            maze->distance(currentLocation, nextCellLocation))
        // Not going to reach the next cell location.
        {
          y.store(nextLocation.first);
          x.store(nextLocation.second);
        } else
        // Not aligned to cell, which means next cell location is definitely
        // valid.
        {
          y.store(nextCellLocation.first);
          x.store(nextCellLocation.second);
        }
      }
    }
  }
}

pacman::Pacman::Orientation pacman::Pacman::getOrientation() {
  return orientation;
}

long pacman::Pacman::getMouthDegrees() { return mouthDegrees; }

// TODO: Make desiredLocation atomic.
void pacman::Pacman::onKeyEvent(int scanCode) {
  switch (scanCode) {
  case SDL_SCANCODE_LEFT:
    desiredOrientation = Orientation::LEFT;
    break;
  case SDL_SCANCODE_RIGHT:
    desiredOrientation = Orientation::RIGHT;
    break;
  case SDL_SCANCODE_UP:
    desiredOrientation = Orientation::UP;
    break;
  case SDL_SCANCODE_DOWN:
    desiredOrientation = Orientation::DOWN;
    break;
  default:
    std::cout << "no-op" << '\n';
  }
}

std::pair<long, long> pacman::Pacman::nextLocation(Orientation orientation) {
  long y = getY();
  long x = getX();
  switch (orientation) {
  case UP:
    y -= velocity;
    break;
  case DOWN:
    y += velocity;
    break;
  case LEFT:
    x -= velocity;
    break;
  case RIGHT:
    x += velocity;
    break;
  }
  return std::make_pair(y, x);
}

std::pair<long, long>
pacman::Pacman::nextCellLocation(Orientation orientation) {
  auto maze = maze_weak_ptr.lock();
  bool isCellAligned = maze->isCellAligned(std::make_pair(getY(), getX()));
  long y = getY(), x = getX();
  long cellHeight = maze->getCellHeight();
  long halfCellHeight = cellHeight / 2;
  long cellWidth = maze->getCellWidth();
  long halfCellWidth = cellWidth / 2;
  switch (orientation) {
  case UP:
    if (isCellAligned) {
      y -= cellHeight;
    } else {
      y = (y - halfCellHeight) / cellHeight * cellHeight + halfCellHeight;
    }
    break;
  case DOWN:
    if (isCellAligned) {
      y += cellHeight;
    } else {
      y = (y + halfCellHeight) / cellHeight * cellHeight + halfCellHeight;
    }
    break;
  case LEFT:
    if (isCellAligned) {
      x -= cellWidth;
    } else {
      x = (x - halfCellWidth) / cellWidth * cellWidth + halfCellWidth;
    }
    break;
  case RIGHT:
    if (isCellAligned) {
      x += cellWidth;
    } else {
      x = (x + halfCellWidth) / cellWidth * cellWidth + halfCellWidth;
    }
    break;
  }
  return std::make_pair(y, x);
}

/** Make pacman printable via std::cout. */
namespace pacman {
std::ostream &operator<<(std::ostream &outputStream,
                         const pacman::Pacman &pacman) {
  outputStream << "pacman{loc[" << pacman.y << ", " << pacman.x << "] vel["
               << pacman.velocity << "] orientation:[" << pacman.orientation
               << "] mouthDegrees:[" << pacman.mouthDegrees
               << "] mouthDirection:[" << pacman.mouthDirection
               << "] << numPelletsEaten:[" << pacman.numPelletsEaten << "]}";
}
} // namespace pacman