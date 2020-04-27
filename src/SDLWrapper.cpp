
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
// #include <SDL2/SDL_timer.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "../include/Game.h"
#include "../include/Ghost.h"
#include "../include/Pacman.h"
#include "../include/SDLWrapper.h"

pacman::SDLWrapper::SDLWrapper(std::weak_ptr<Maze> maze_ptr,
                               std::weak_ptr<Pacman> pacman_weak_ptr,
                               std::vector<std::weak_ptr<Ghost>> ghosts,
                               std::string imgFolderPath,
                               std::weak_ptr<Game> game_weak_ptr)
    : pacman_weak_ptr(pacman_weak_ptr), ghosts(ghosts),
      imgFolderPath(imgFolderPath), game_weak_ptr(game_weak_ptr) {
  auto maze = maze_ptr.lock();
  mazeMatrix = maze->getMazeMatrix();
  cellHeight = maze->getCellHeight();
  cellWidth = maze->getCellWidth();
  numRows = maze->getMazeMatrix().lock()->size();
  numCols = maze->getMazeMatrix().lock()->begin()->size();
  initSDL();
  initWindow();
  initRenderer();
  initTextures();
}

pacman::SDLWrapper::~SDLWrapper() {
  SDL_DestroyTexture(pelletTexture);
  for (auto iter = pacmanTextures.begin(); iter < pacmanTextures.end();
       iter++) {
    SDL_DestroyTexture(*iter);
  }
  SDL_DestroyWindow(sdlWindow);
  IMG_Quit();
  SDL_Quit();
}

pacman::SDLWrapper::SDLWrapper(const SDLWrapper& other) {
  // Re-use assignment operator, as same operators are performed.
  *this = other;
}

pacman::SDLWrapper& pacman::SDLWrapper::operator=(const SDLWrapper& other) {
  if (this == &other) {
    return *this;
  }
  initSDL();
  initWindow();
  initRenderer();
  initTextures();
  game_weak_ptr = other.game_weak_ptr;
  mazeMatrix = other.mazeMatrix;
  pacman_weak_ptr = other.pacman_weak_ptr;
  ghosts = other.ghosts;
  cellHeight = other.cellHeight;
  cellWidth = other.cellWidth;
  numRows = other.numRows;
  numCols = other.numCols;
  imgFolderPath = other.imgFolderPath;
  return *this;
}

void pacman::SDLWrapper::processInputEvents() {

  // process events until event queue is empty.
  while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_QUIT) {
      game_weak_ptr.lock()->stop("Quitting due to user input.");
      pacman_weak_ptr.lock()->stop();
    } else if (e.type == SDL_KEYDOWN) {
      if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        game_weak_ptr.lock()->stop("Quitting due to user input.");
        pacman_weak_ptr.lock()->stop();
        for (auto ghost_iter = ghosts.begin(); ghost_iter < ghosts.end();
             ghost_iter++) {
          (*ghost_iter).lock()->stop();
        }
      } else {
        pacman_weak_ptr.lock()->onKeyEvent(e.key.keysym.scancode);
      }
    }
  }
}

void pacman::SDLWrapper::render() {
  SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(sdlRenderer);
  renderMaze();
  renderPacman();
  renderGhosts();
  SDL_RenderPresent(sdlRenderer);
}

void pacman::SDLWrapper::initSDL() {
  maybeThrowRuntimeError(SDL_Init(SDL_INIT_VIDEO) != 0,
                         "Error initializing SDL: \n" +
                             std::string(SDL_GetError()));
  int imgLoadConfigBits = IMG_INIT_PNG;
  maybeThrowRuntimeError(!IMG_Init(imgLoadConfigBits) & imgLoadConfigBits,
                         "Error initializing SDL_image: \n" +
                             std::string(IMG_GetError()));
}

void pacman::SDLWrapper::initWindow() {
  sdlWindow = SDL_CreateWindow("Pac that man!", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, cellWidth * numCols,
                               cellHeight * numRows, SDL_WINDOW_SHOWN);
  maybeThrowRuntimeError(!sdlWindow, "Error initializing SDL Window: \n" +
                                         std::string(SDL_GetError()));
}

void pacman::SDLWrapper::initRenderer() {
  Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, render_flags);
  maybeThrowRuntimeError(!sdlRenderer, "Error initializing SDL renderer: \n" +
                                           std::string(SDL_GetError()));
}

void pacman::SDLWrapper::initTextures() {
  // Window Texture
  SDL_Surface *sdlSurface = SDL_GetWindowSurface(sdlWindow);
  maybeThrowRuntimeError(!sdlSurface,
                         "Error retrieving SDL window surface: \n" +
                             std::string(SDL_GetError()));
  sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, sdlSurface);
  SDL_FreeSurface(sdlSurface);
  maybeThrowRuntimeError(!sdlTexture, "Error initializing SDL texture: \n" +
                                          std::string(SDL_GetError()));

  // Pellet Texture
  std::string pelletFilePath = imgFolderPath + "/pellet.png";
  SDL_Surface *pelletSurface = IMG_Load(pelletFilePath.c_str());
  maybeThrowRuntimeError(pelletSurface == NULL,
                         "Error initializing pellet surface: \n" +
                             std::string(SDL_GetError()));
  pelletTexture = SDL_CreateTextureFromSurface(sdlRenderer, pelletSurface);
  SDL_FreeSurface(pelletSurface);
  maybeThrowRuntimeError(!sdlTexture, "Error initializing pellet texture: \n" +
                                          std::string(SDL_GetError()));

  // Pacman Textures
  std::vector<std::string> pacmanImagePaths{
      "/pacman-agent-0.png", "/pacman-agent-1.png", "/pacman-agent-2.png",
      "/pacman-agent-3.png", "/pacman-agent-4.png"};
  for (auto iter = pacmanImagePaths.begin(); iter < pacmanImagePaths.end();
       iter++) {
    SDL_Surface *pacmanSurface = IMG_Load((imgFolderPath + *iter).c_str());
    maybeThrowRuntimeError(pacmanSurface == NULL,
                           "Error initializing pacman surface: \n" +
                               std::string(SDL_GetError()));
    SDL_Texture *pacmanTexture =
        SDL_CreateTextureFromSurface(sdlRenderer, pacmanSurface);
    pacmanTextures.push_back(pacmanTexture);
    SDL_FreeSurface(pacmanSurface);
    maybeThrowRuntimeError(!pacmanTexture,
                           "Error initializing pacman texture: \n" +
                               std::string(SDL_GetError()));
  }

  // Ghost Textures
  std::vector<std::string> ghostImagePaths{"/blinky-0.png", "/clyde-0.png",
                                           "/inky-0.png", "/pinky-0.png"};
  for (auto iter = ghostImagePaths.begin(); iter < ghostImagePaths.end();
       iter++) {
    SDL_Surface *ghostSurface = IMG_Load((imgFolderPath + *iter).c_str());
    maybeThrowRuntimeError(ghostSurface == NULL,
                           "Error initializing ghost surface: \n" +
                               std::string(SDL_GetError()));
    SDL_Texture *ghostTexture =
        SDL_CreateTextureFromSurface(sdlRenderer, ghostSurface);
    ghostTextures.push_back(ghostTexture);
    SDL_FreeSurface(ghostSurface);
    maybeThrowRuntimeError(!ghostTexture,
                           "Error initializing ghost texture: \n" +
                               std::string(SDL_GetError()));
  }
}

void pacman::SDLWrapper::renderMaze() {

  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      SDL_Rect block;
      block.y = i * cellHeight;
      block.x = j * cellWidth;
      block.h = cellHeight;
      block.w = cellWidth;
      switch (mazeMatrix.lock()->at(i).at(j)) {
      case Maze::Cell::kBorder:
        SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(sdlRenderer, &block);
        break;
      case Maze::Cell::kPellet:
        scale(block, 0.3);
        SDL_RenderCopy(sdlRenderer, pelletTexture, NULL, &block);
        break;
      case Maze::Cell::kEmpty:
        SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(sdlRenderer, &block);
        break;
      }
    }
  }
}

void pacman::SDLWrapper::renderPacman() {
  auto pacman_shared = pacman_weak_ptr.lock();
  long y = pacman_shared->getY(), x = pacman_shared->getX();

  SDL_Rect block;
  block.y = y - cellHeight / 2;
  block.x = x - cellHeight / 2;
  block.h = cellHeight;
  block.w = cellWidth;
  long mouthDegrees =
      pacman_shared
          ->getMouthDegrees(); // TODO: Make this thread-safe. Just atomic_long
  SDL_RenderCopyEx(
      sdlRenderer, mouthDegreesToTexture(pacman_shared->getMouthDegrees()),
      NULL, &block, orientationToDegrees(pacman_shared->getOrientation()), NULL,
      SDL_RendererFlip::SDL_FLIP_NONE);
}

void pacman::SDLWrapper::renderGhosts() {
  for (int i = 0; i < ghosts.size(); i++) {
    auto ghost = ghosts.at(i).lock();
    long y = ghost->getY(), x = ghost->getX();
    SDL_Rect block;
    block.y = y - cellHeight / 2;
    block.x = x - cellHeight / 2;
    block.h = cellHeight;
    block.w = cellWidth;
    SDL_RenderCopyEx(sdlRenderer, ghostTextures.at(i), NULL, &block, 0, NULL,
                     SDL_RendererFlip::SDL_FLIP_NONE);
  }
}

long pacman::SDLWrapper::orientationToDegrees(Pacman::Orientation orientation) {
  switch (orientation) {
  case Pacman::Orientation::UP:
    return 270;
  case Pacman::Orientation::DOWN:
    return 90;
  case Pacman::Orientation::LEFT:
    return 180;
  case Pacman::Orientation::RIGHT:
    return 0;
  }
}

SDL_Texture *pacman::SDLWrapper::mouthDegreesToTexture(long mouthDegrees) {
  if (mouthDegrees < 24) {
    return pacmanTextures.at(0);
  } else if (mouthDegrees < 48) {
    return pacmanTextures.at(1);
  } else if (mouthDegrees < 72) {
    return pacmanTextures.at(2);
  } else if (mouthDegrees, 96) {
    return pacmanTextures.at(3);
  } else {
    return pacmanTextures.at(4);
  }
}

std::string pacman::SDLWrapper::getPacmanImage() {
  return "/pacman-agent-2.png";
}

void pacman::SDLWrapper::maybeThrowRuntimeError(bool throwError,
                                                std::string message) {
  if (throwError) {
    std::cout << message << '\n';
    throw std::runtime_error(message);
  }
}

void pacman::SDLWrapper::SDLWrapper::scale(SDL_Rect &rect, float scale) {
  long height = static_cast<long>(rect.h * scale);
  long width = static_cast<long>(rect.w * scale);
  rect.y += rect.h / 2 - height / 2;
  rect.x += rect.w / 2 - width / 2;
  rect.h = height;
  rect.w = width;
}
