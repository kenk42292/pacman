
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
// #include <SDL2/SDL_timer.h>
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>

#include "../../include/ui/SDLWrapper.h"
#include "../../include/Pacman.h"
#include "../../include/ui/KeyEventListener.h"

pacman::ui::SDLWrapper::SDLWrapper(std::shared_ptr<Maze> maze,
                                   std::shared_ptr<Pacman> pacman,
                                   std::vector<std::shared_ptr<Ghost>> ghosts,
                                   std::string imgFolderPath)
                                   : mazeMatrix(maze->getMazeMatrix()),
                                     pacman(pacman),
                                     ghosts(ghosts),
                                     cellHeight(maze->getCellHeight()),
                                     cellWidth(maze->getCellWidth()),
                                     numRows(maze->getMazeMatrix()->size()),
                                     numCols(maze->getMazeMatrix()->begin()->size()),
                                     imgFolderPath(imgFolderPath) {
    initSDL();
    initWindow();
    initRenderer();
    initTextures();
    // SDL_UpdateWindowSurface(sdlWindow);
    std::cout << "numRows: " << numRows << '\n';
    std::cout << "numCols: " << numCols << '\n';
}

pacman::ui::SDLWrapper::~SDLWrapper() {
    SDL_DestroyTexture(pelletTexture);
    SDL_DestroyTexture(pacmanTexture);
    SDL_DestroyWindow(sdlWindow);
    IMG_Quit();
    SDL_Quit();
}

void pacman::ui::SDLWrapper::start() {

    while (!quit) {
        // std::cout << "sdlWrapper thread: " << std::this_thread::get_id() << "\n";
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        // process events until event queue is empty.
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
                pacman->stop();
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    quit = true;
                    pacman->stop();
                } else {
                    notifyKeyEventListeners(e.key.keysym.scancode);
                }
            }
        }
        SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sdlRenderer);
        renderMaze();
        renderPacman();
        // renderGhosts();
        SDL_RenderPresent(sdlRenderer);
    }
}

void pacman::ui::SDLWrapper::addKeyEventListener(std::shared_ptr<KeyEventListener> listener) {
    keyEventListeners.push_back(listener);
}

void pacman::ui::SDLWrapper::initSDL() {
    maybeThrowRuntimeError(SDL_Init(SDL_INIT_VIDEO) != 0, "Error initializing SDL: \n" + std::string(SDL_GetError()));
    int imgLoadConfigBits = IMG_INIT_PNG;
    maybeThrowRuntimeError(!IMG_Init(imgLoadConfigBits) & imgLoadConfigBits, "Error initializing SDL_image: \n" + std::string(IMG_GetError()));
}

void pacman::ui::SDLWrapper::initWindow() {
    sdlWindow = SDL_CreateWindow("Pac that man!",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                cellWidth*numCols,
                                cellHeight*numRows,
                                SDL_WINDOW_SHOWN);
    maybeThrowRuntimeError(!sdlWindow, "Error initializing SDL Window: \n" + std::string(SDL_GetError()));
}

void pacman::ui::SDLWrapper::initRenderer() {
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, render_flags);
    maybeThrowRuntimeError(!sdlRenderer, "Error initializing SDL renderer: \n" + std::string(SDL_GetError()));
}

void pacman::ui::SDLWrapper::initTextures() {
    // Window Texture
    SDL_Surface* sdlSurface = SDL_GetWindowSurface(sdlWindow);
    maybeThrowRuntimeError(!sdlSurface, "Error retrieving SDL window surface: \n" + std::string(SDL_GetError()));
    sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, sdlSurface);
    SDL_FreeSurface(sdlSurface);
    maybeThrowRuntimeError(!sdlTexture, "Error initializing SDL texture: \n" + std::string(SDL_GetError()));

    // Pellet Texture
    std::string pelletFilePath = imgFolderPath + "/pellet.png";
    SDL_Surface* pelletSurface = IMG_Load(pelletFilePath.c_str());
    maybeThrowRuntimeError(pelletSurface == NULL, "Error initializing pellet surface: \n" + std::string(SDL_GetError()));
    pelletTexture = SDL_CreateTextureFromSurface(sdlRenderer, pelletSurface);
    SDL_FreeSurface(pelletSurface);
    maybeThrowRuntimeError(!sdlTexture, "Error initializing pellet texture: \n" + std::string(SDL_GetError()));

    // Pacman Texture
    std::string pacmanFilePath = imgFolderPath + getPacmanImage();
    SDL_Surface* pacmanSurface = IMG_Load(pacmanFilePath.c_str());
    maybeThrowRuntimeError(pacmanSurface == NULL, "Error initializing pacman surface: \n" + std::string(SDL_GetError()));
    pacmanTexture = SDL_CreateTextureFromSurface(sdlRenderer, pacmanSurface);
    SDL_FreeSurface(pacmanSurface);
    maybeThrowRuntimeError(!sdlTexture, "Error initializing pacman texture: \n" + std::string(SDL_GetError()));

}

void pacman::ui::SDLWrapper::renderMaze() {
    
    for (int i=0; i<numRows; i++) {
        for (int j=0; j<numCols; j++) {
            SDL_Rect block;
            block.y = i*cellHeight;
            block.x = j*cellWidth;
            block.h = cellHeight;
            block.w = cellWidth;
            switch (mazeMatrix->at(i).at(j)) {
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

void pacman::ui::SDLWrapper::renderPacman() {
    long y = pacman->y, x = pacman->x;
    
    SDL_Rect block;
    block.y = y-cellHeight/2;
    block.x = x-cellHeight/2;
    block.h = cellHeight;
    block.w = cellWidth;
    SDL_RenderCopy(sdlRenderer, pacmanTexture, NULL, &block);
}

void pacman::ui::SDLWrapper::notifyKeyEventListeners(int scanCode) {
    for (std::vector<std::shared_ptr<pacman::ui::KeyEventListener>>::iterator iter=keyEventListeners.begin(); iter < keyEventListeners.end(); iter++) {
        iter->get()->onKeyEvent(scanCode);
    }
}

std::string pacman::ui::SDLWrapper::getPacmanImage() {
    return "/pacman-agent-2.png";
}

void pacman::ui::SDLWrapper::maybeThrowRuntimeError(bool throwError, std::string message) {
    if (throwError) {
        std::cout << message << '\n';
        throw std::runtime_error(message);
    }
}

void pacman::ui::SDLWrapper::SDLWrapper::scale(SDL_Rect & rect, float scale) {
    long height = static_cast<long>(rect.h * scale);
    long width = static_cast<long>(rect.w * scale);
    rect.y += rect.h/2 - height/2;
    rect.x += rect.w/2 - width/2;
    rect.h = height;
    rect.w = width;

}

