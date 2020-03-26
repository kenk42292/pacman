#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include "../include/Cell.h"
#include "../include/Game.h"
#include "../include/ui/SDLWrapper.h"

pacman::Game::Game(std::string gameConfigFolderPath, std::string imgFolderPath) {
    initMaze(gameConfigFolderPath + "/maze");
    initAgents(gameConfigFolderPath + "/agents");
    std::cout << "pacman: " << *pacman << '\n';
    std::for_each (ghosts.begin(), ghosts.end(), [](std::shared_ptr<Ghost>& ghost) {
        std::cout << "ghost: " << *(ghost.get()) << '\n';
    });

    // maze->
    initUI(imgFolderPath);
}

void pacman::Game::start() {
    sdlWrapper->start();
}

void pacman::Game::initMaze(std::string mazeConfigPath) {
    auto mazeMatrix = readMazeMatrix(mazeConfigPath);
    maze = std::make_shared<Maze>(mazeMatrix, CELL_HEIGHT, CELL_WIDTH);
}

void pacman::Game::initAgents(std::string agentsConfigPath) {
    long y, x;

    auto pacmanIndicesVector = parseIndicesByPrefix(agentsConfigPath, PACMAN);
    if (pacmanIndicesVector.size() == 0) {
        throw std::invalid_argument("No pacman location found.");
    }
    auto pacmanIndeces = pacmanIndicesVector.at(0);
    y = indexToLocation(pacmanIndeces.first, CELL_HEIGHT);
    x = indexToLocation(pacmanIndeces.second, CELL_WIDTH);
    pacman = std::make_shared<Pacman>(y, x);

    auto ghostIndicesVector = parseIndicesByPrefix(agentsConfigPath, GHOST);
    for (auto ghostIndices : ghostIndicesVector) {
        y = indexToLocation(ghostIndices.first, CELL_HEIGHT);
        x = indexToLocation(ghostIndices.second, CELL_WIDTH);
        ghosts.push_back(std::make_shared<Ghost>(y, x));
    }
}

void pacman::Game::initUI(std::string imgFolderPath) {
    // Start User interaction with components of the game.
    // pacman::ui::SDLWrapper sdlWrapper(game, maze, pacman, ghosts);
    sdlWrapper = std::make_unique<ui::SDLWrapper>(maze, pacman, ghosts, imgFolderPath);
}

std::shared_ptr<std::vector<std::vector<pacman::Cell>>> pacman::Game::readMazeMatrix(std::string mazeFilePath) {

    std::ifstream inputFileStream(mazeFilePath);
    if (!inputFileStream.is_open()) {
        std::cout << "Failed to read in input file[" << mazeFilePath << "]" << '\n';
        exit(1);
    }

    auto mazeMatrix = std::make_shared<std::vector<std::vector<pacman::Cell>>>();

    std::string line;
    while (std::getline(inputFileStream, line)) {
        std::vector<Cell> row;
        for (auto it=line.begin() ; it != line.end(); ++it) {
            row.push_back(charToCell(*it));
        }

        mazeMatrix->push_back(row);
    }

    inputFileStream.close();

    return mazeMatrix;
}

pacman::Cell pacman::Game::charToCell(char & c) {
    switch (c) {
        case '|':
        case '+':
        case '-':
        case '_':
        case 'x':
            return pacman::Cell::kBorder;
        case '.':
            return pacman::Cell::kPellet;
        case ' ':
            return pacman::Cell::kEmpty;
        default:
            std::cout << "Invalid char[" << std::string(1, c) << "] to be converted to cell." << '\n';
            throw std::invalid_argument("Invalid char[" + std::string(1, c) + "] to be converted to cell.");
    }
}

std::vector<std::pair<int, int>> pacman::Game::parseIndicesByPrefix(std::string agentsConfigPath, std::string agent) {
    if (agent != PACMAN && agent != GHOST) {
        throw std::invalid_argument("Agent must be either pacman or ghost.");
    }

    std::ifstream agentsConfigFileStream(agentsConfigPath);
    if (!agentsConfigFileStream.is_open()) {
        std::cout << "Failed to read in input file[" << agentsConfigPath << "]" << '\n';
        exit(1);
    }

    std::vector<std::pair<int, int>> result;
    std::string line;
    while (std::getline(agentsConfigFileStream, line)) {
        // remove spaces
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

        if (line.rfind(agent, 0) == 0)
        {
            std::string locStr = line.substr(agent.length() + 1);
            int delimIndex = locStr.find(DELIM);
            int i = stol(locStr.substr(0, delimIndex));
            int j = stol(locStr.substr(delimIndex+1));
            result.push_back(std::make_pair(i, j));
        }
    }

    return result;
}

long pacman::Game::indexToLocation(int index, long scale) {
    return index*scale + scale/2;
}
