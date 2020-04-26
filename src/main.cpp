
#include "../include/Game.h"

int main(void)
{    
    // TODO: Update to be non-absolute, so it can run on other computers...
    std::string gameConfigFolderPath = "/home/ken/udacity/pacman/res/gameconfigs/game1";
    std::string imgFolderPath = "/home/ken/udacity/pacman/res/img";

    // Must create shared_ptr to this intended singleton, to allow use of shared_from_this.
    auto game = pacman::Game::create(gameConfigFolderPath, imgFolderPath);
    game->start();
    return 0;
}