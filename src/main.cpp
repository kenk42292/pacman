
#include <signal.h>

#include "../include/Game.h"

std::shared_ptr<pacman::Game> game;

/** Force-quit call-back. Register for when user uses Ctrl-C (or something
 * similar) to terminate program. This is needed to prevent rogue threads from
 * becoming a zombie.*/
void signal_callback(int signum) {
  game->stop("Signal[" + std::to_string(signum) + "] caught.");
}

int main(int argc, char** argv) {

  // Register handler for force-quitting via Ctrl-C (or similar).
  signal(SIGINT, signal_callback);

  // Hard-coded paths to resources.
  std::string gameConfigFolderPath =
      "../res/gameconfigs/game5";
  std::string imgFolderPath = "../res/img";

  // If user specified which game config is desired.
  if (argc == 2) {
    std::string configNum = std::string(argv[1]);
    if (1 <= stoi(configNum) && stoi(configNum) <= 5) {
      gameConfigFolderPath = "../res/gameconfigs/game" + configNum;
    }
  }

  // Create and start game.
  game = pacman::Game::create(gameConfigFolderPath, imgFolderPath);
  game->start();
  return 0;
}
