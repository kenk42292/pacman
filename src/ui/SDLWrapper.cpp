
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
// #include <SDL2/SDL_timer.h>
#include <iostream>

#include "../../include/Cell.h"
#include "../../include/ui/SDLWrapper.h"

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
    initTexture();
    initDotTexture();
    // SDL_UpdateWindowSurface(sdlWindow);
    std::cout << "numRows: " << numRows << '\n';
    std::cout << "numCols: " << numCols << '\n';
}

pacman::ui::SDLWrapper::~SDLWrapper() {
    SDL_DestroyTexture(dotTexture);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

void pacman::ui::SDLWrapper::start() {

    while (!quit) {
        // process events until event queue is empty.
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                notifyKeyEventListeners(e.key.keysym.scancode);
            }
        }
        SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sdlRenderer);
        // SDL_RenderClear(sdlRenderer);
        renderMaze();
        // renderPacman();
        // renderGhosts();
        SDL_RenderPresent(sdlRenderer);
    }
}

void pacman::ui::SDLWrapper::addKeyEventListener(std::shared_ptr<KeyEventListener> listener) {
    keyEventListeners.push_back(listener);
}

void pacman::ui::SDLWrapper::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error("Error initializing SDL: %s\n" + std::string(SDL_GetError()));
    }
}

void pacman::ui::SDLWrapper::initWindow() {
    sdlWindow = SDL_CreateWindow("Pac that man!",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                cellWidth*numCols,
                                cellHeight*numRows,
                                SDL_WINDOW_SHOWN);
    if (!sdlWindow)
    {
        throw std::runtime_error("Error initializing SDL Window: %s\n" + std::string(SDL_GetError()));
    }
}

void pacman::ui::SDLWrapper::initRenderer() {
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, render_flags);
    if (!sdlRenderer)
    {
        throw std::runtime_error("Error initializing SDL renderer: %s\n" + std::string(SDL_GetError()));
    }
}

void pacman::ui::SDLWrapper::initTexture() {
    SDL_Surface* sdlSurface = SDL_GetWindowSurface(sdlWindow);
    if (!sdlSurface)
    {
        throw std::runtime_error("Error retrieving SDL window surface: %s\n" + std::string(SDL_GetError()));
    }
    sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, sdlSurface);
    SDL_FreeSurface(sdlSurface);
    if (!sdlTexture)
    {
        throw std::runtime_error("Error initializing SDL texture: %s\n" + std::string(SDL_GetError()));
    }
}

void pacman::ui::SDLWrapper::initDotTexture() {
    std::string dotFilePath = imgFolderPath + "/dot.bmp";
    SDL_Surface* dotSurface = IMG_Load(dotFilePath.c_str());
    if (!dotSurface)
    {
        throw std::runtime_error("Error initializing dot surface: %s\n" + std::string(SDL_GetError()));
    }
    dotTexture = SDL_CreateTextureFromSurface(sdlRenderer, dotSurface);
    SDL_FreeSurface(dotSurface);
    if (!sdlTexture)
    {
        throw std::runtime_error("Error initializing dot texture: %s\n" + std::string(SDL_GetError()));
    }
}

void pacman::ui::SDLWrapper::renderMaze() {
    
    for (int i=0; i<numRows; i++) {
        for (int j=0; j<numCols; j++) {
            SDL_Rect block;
            block.y = i*cellHeight;
            block.x = j*cellWidth;
            block.h = cellHeight;
            block.w = cellWidth;
            long halfCellHeight = cellHeight/2;
            long halfCellWidth = cellWidth/2;
            switch (mazeMatrix->at(i).at(j)) {
                case kBorder:
                    SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0x00, 0x00, 0xFF);
                    break;
                case kPellet:
                    // SDL_RenderCopy(sdlRenderer, dotTexture, NULL, &block);
                    block.y += halfCellHeight;
                    block.x += halfCellWidth;
                    block.h = cellHeight/10;
                    block.w = cellHeight/10;
                    SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0x00, 0x00, 0xFF);
                    break;
                case kEmpty:
                    SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
            SDL_RenderFillRect(sdlRenderer, &block);
        }
    }
}

void pacman::ui::SDLWrapper::notifyKeyEventListeners(int scanCode) {
    for (auto iter=keyEventListeners.begin(); iter < keyEventListeners.end(); iter++) {
        iter->get()->onKeyEvent(scanCode);
    }
}



