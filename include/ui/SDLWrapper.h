#ifndef PACMAN_UI_SDLWRAPPER_H_
#define PACMAN_UI_SDLWRAPPER_H_

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <string>
#include <vector>

#include "../../include/ui/KeyEventListener.h"
#include "../../include/Maze.h"

namespace pacman {
namespace ui {

class SDLWrapper {
public:
    SDLWrapper(std::shared_ptr<Maze> maze, std::shared_ptr<Pacman> pacman, std::vector<std::shared_ptr<Ghost>> ghosts, std::string imgFolderPath);
    ~SDLWrapper();
    void start();
    void addKeyEventListener(std::shared_ptr<KeyEventListener> listener);
private:
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    SDL_Texture* sdlTexture;
    SDL_Texture* dotTexture;
    bool quit = false;
    SDL_Event e;
    std::vector<std::shared_ptr<KeyEventListener>> keyEventListeners;
    std::shared_ptr<std::vector<std::vector<Cell>>> mazeMatrix;
    std::shared_ptr<Pacman> pacman;
    std::vector<std::shared_ptr<Ghost>> ghosts;
    long cellHeight;
    long cellWidth;
    int numRows;
    int numCols;
    std::string imgFolderPath;
    void initSDL();
    void initWindow();
    void initRenderer();
    void initTexture();
    void initDotTexture();
    void renderMaze();
    void renderPacman();
    void renderGhosts();
    void notifyKeyEventListeners(int scanCode);
};

} // ui
} // pacman

#endif // PACMAN_UI_SDLWRAPPER_H_
