
#include "../include/Game.h"

int main(void)
{    
    // TODO: Update to be non-absolute, so it can run on other computers...
    std::string gameConfigFolderPath = "/home/ken/udacity/pacman/res/gameconfigs/game1";
    std::string imgFolderPath = "/home/ken/udacity/pacman/res/img";
    pacman::Game game(gameConfigFolderPath, imgFolderPath);
    game.start();

    return 0;
}