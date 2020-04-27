#ifndef PACMAN_SDLWRAPPER_H_
#define PACMAN_SDLWRAPPER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <memory>
#include <string>
#include <vector>

#include "Maze.h"
#include "Pacman.h"

namespace pacman {

class Game;

class SDLWrapper {
public:
  SDLWrapper(std::weak_ptr<Maze> maze, std::weak_ptr<Pacman> pacman_weak_ptr,
             std::vector<std::weak_ptr<Ghost>> ghosts,
             std::string imgFolderPath, std::weak_ptr<Game> game_weak_ptr);
  ~SDLWrapper();
  SDLWrapper(const SDLWrapper& other);
  SDLWrapper& operator=(const SDLWrapper& other);

  /** Process any UI input events that were fed in since last time this method
   * was run. */
  void processInputEvents();

  /** Rendering current state of game. */
  void render();

private:
  // SDL UI components.
  SDL_Window *sdlWindow;
  SDL_Renderer *sdlRenderer;
  SDL_Texture *sdlTexture;
  SDL_Texture *pelletTexture;
  std::vector<SDL_Texture *> pacmanTextures;
  std::vector<SDL_Texture *> ghostTextures;

  // User input.
  SDL_Event e;

  // Game components to pull info from, to display.
  std::weak_ptr<Game> game_weak_ptr;
  std::weak_ptr<std::vector<std::vector<Maze::Cell>>> mazeMatrix;
  std::weak_ptr<Pacman> pacman_weak_ptr;
  std::vector<std::weak_ptr<Ghost>> ghosts;
  long cellHeight;
  long cellWidth;
  int numRows;
  int numCols;

  // Path to folder that contains resources needed to render game.
  std::string imgFolderPath;

  // Initialization of SDL components.
  void initSDL();
  void initWindow();
  void initRenderer();
  void initTextures();

  // Subtasks for rendering UI.
  void renderMaze();
  void renderPacman();
  void renderGhosts();

  // Some Helpers.
  void maybeThrowRuntimeError(bool throwError, std::string message);
  void scale(SDL_Rect &rect, float scale);
  std::string getPacmanImage();
  long orientationToDegrees(Pacman::Orientation orientation);
  SDL_Texture *mouthDegreesToTexture(long mouthDegrees);
};

} // namespace pacman

#endif // PACMAN_SDLWRAPPER_H_
