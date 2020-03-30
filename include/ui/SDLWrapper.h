#ifndef PACMAN_UI_SDLWRAPPER_H_
#define PACMAN_UI_SDLWRAPPER_H_

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <string>
#include <vector>

#include "../Maze.h"

namespace pacman {
namespace ui {

class KeyEventListener;

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
    SDL_Texture* pelletTexture;
    SDL_Texture* pacmanTexture;

    // SDL_Texture* pacmanTexture;
    // std::vector<SDL_Texture*> ghostTextures;
    bool quit = false;
    SDL_Event e;
    std::vector<std::shared_ptr<KeyEventListener>> keyEventListeners;
    std::shared_ptr<std::vector<std::vector<Maze::Cell>>> mazeMatrix;
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
    void initTextures();
    void renderMaze();
    void renderPacman();
    void renderGhosts();
    void notifyKeyEventListeners(int scanCode);
    void maybeThrowRuntimeError(bool throwError, std::string message);
    void scale(SDL_Rect & rect, float scale);
    std::string getPacmanImage();
};

} // ui
} // pacman

#endif // PACMAN_UI_SDLWRAPPER_H_
