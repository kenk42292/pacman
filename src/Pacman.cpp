
#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "../include/Pacman.h"
#include "../include/Maze.h"

pacman::Pacman::Pacman(long y, long x, std::shared_ptr<Maze> maze)
    : Agent(y, x, PACMAN_VELOCITY, maze), orientation(UP), desiredOrientation(UP), mouthDegrees(0) {}

void pacman::Pacman::start() {
  Agent::start();
  while (alive.load()) {
    // std::cout << "pacman thread: " << *this << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    std::pair<long, long> currentLocation = std::make_pair(y, x);
    std::pair<long, long> nextDesiredLocation = this->nextLocation(desiredOrientation);
    std::pair<long, long> nextLocation = this->nextLocation(orientation);
    std::pair<long, long> nextCellLocation = this->nextCellLocation(orientation);
    if (orientation == desiredOrientation) {
      if (maze->isValid(nextDesiredLocation))
      // If pacman can move straight to next desired location, do so.
      {
        y = nextLocation.first;
        x = nextLocation.second;
      }
      else if (maze->isValid(nextCellLocation))
      // Otherwise, move until flush with cell we're currently entering.
      {
        y = nextCellLocation.first;
        x = nextCellLocation.second;
      }
    } else {
      if (maze->isCellAligned(currentLocation)) {
        if (maze->isValid(nextDesiredLocation)) {
          orientation = desiredOrientation;
          y = nextDesiredLocation.first;
          x = nextDesiredLocation.second;
        } else if (maze->isValid(nextLocation)) {
          y = nextLocation.first;
          x = nextLocation.second;
        } else if (maze->isValid(nextCellLocation)) {
          y = nextCellLocation.first;
          x = nextCellLocation.second;
        }
      } else {
        // TODO: Refine, if it jitters when stopping at cells
        if (maze->distance(currentLocation, nextLocation) < maze->distance(currentLocation, nextCellLocation))
        // Not going to reach the next cell location.
        {
          y = nextLocation.first;
          x = nextLocation.second;
        }
        else
        // Not aligned to cell, which means next cell location is definitely valid.
        {
          y = nextCellLocation.first;
          x = nextCellLocation.second;
        }
      }
    }
  }
}

// TODO: Make desiredLocation atomic.
void pacman::Pacman::onKeyEvent(int scanCode) {
  switch (scanCode) {
  case SDL_SCANCODE_LEFT:
    std::cout << "LEFT" << '\n';
    desiredOrientation = Orientation::LEFT;
    break;
  case SDL_SCANCODE_RIGHT:
    std::cout << "RIGHT" << '\n';
    desiredOrientation = Orientation::RIGHT;
    break;
  case SDL_SCANCODE_UP:
    std::cout << "UP" << '\n';
    desiredOrientation = Orientation::UP;
    break;
  case SDL_SCANCODE_DOWN:
    std::cout << "DOWN" << '\n';
    desiredOrientation = Orientation::DOWN;
    break;
  default:
    std::cout << "no-op" << '\n';
  }
}

std::pair<long, long> pacman::Pacman::nextLocation(Orientation orientation) {
  // long y = this->y, x = this->x;
  long y = this->y;
  long x = this->x;
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

std::pair<long, long> pacman::Pacman::nextCellLocation(Orientation orientation) {
  bool isCellAligned = maze->isCellAligned(std::make_pair(this->y, this->x));
  long y = this->y, x = this->x;
  long cellHeight = maze->getCellHeight();
  long halfCellHeight = cellHeight/2;
  long cellWidth = maze->getCellWidth();
  long halfCellWidth = cellWidth/2;
  switch (orientation) {
    case UP:
      if (isCellAligned) {
        y -= cellHeight;
      } else {
        y = (y-halfCellHeight)/cellHeight*cellHeight+halfCellHeight;
      }
      break;
    case DOWN:
      if (isCellAligned) {
        y += cellHeight;
      } else {
        y = (y+halfCellHeight)/cellHeight*cellHeight+halfCellHeight;
      }
      break;
    case LEFT:
      if (isCellAligned) {
        x -= cellWidth;
      } else {
        x = (x-halfCellWidth)/cellWidth*cellWidth+halfCellWidth;
      }
      break;
    case RIGHT:
      if (isCellAligned) {
        x += cellWidth;
      } else {
        x = (x+halfCellWidth)/cellWidth*cellWidth+halfCellWidth;
      }
      break;
  }
  return std::make_pair(y, x);
}

/** Make pacman printable via std::cout. */
namespace pacman {
std::ostream &operator<<(std::ostream &outputStream,
                         const pacman::Pacman &pacman) {
  outputStream << "pacman{loc[" << pacman.y << ", " << pacman.x
               << "] vel[" << pacman.velocity << "] orientation:["
               << pacman.orientation << "]}";
}
} // namespace pacman