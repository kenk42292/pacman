
#include <signal.h>

#include "../include/Game.h"

std::shared_ptr<pacman::Game> game;

/** Force-quit call-back. Register for when user uses Ctrl-C (or something
 * similar) to terminate program. This is needed to prevent rogue threads from
 * becoming a zombie.*/
void signal_callback(int signum) {
  game->stop("Signal[" + std::to_string(signum) + "] caught.");
}

int main(void) {

  // Register handler for force-quitting via Ctrl-C (or similar).
  signal(SIGINT, signal_callback);

  // Hard-coded paths to resources.
  std::string gameConfigFolderPath =
      "../res/gameconfigs/game1";
  std::string imgFolderPath = "../res/img";

  // Create and start game.
  game = pacman::Game::create(gameConfigFolderPath, imgFolderPath);
  game->start();
  return 0;
}
