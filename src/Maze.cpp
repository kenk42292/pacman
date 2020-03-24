#include <fstream>
#include <string>
#include <iostream>

#include "../include/Cell.h"
#include "../include/Maze.h"

pacman::Maze::Maze(std::string mazeConfigPath, long cellHeight, long cellWidth)
    : cellHeight(cellHeight), cellWidth(cellWidth) {
    mazeMatrix = readMazeMatrix(mazeConfigPath);
}

pacman::Maze::~Maze() {

}

long pacman::Maze::getCellHeight() {
    return cellHeight;
}

long pacman::Maze::getCellWidth() {
    return cellWidth;
}

std::shared_ptr<std::vector<std::vector<pacman::Cell>>> pacman::Maze::getMazeMatrix() {
    return mazeMatrix;
}

std::shared_ptr<std::vector<std::vector<pacman::Cell>>> pacman::Maze::readMazeMatrix(std::string mazeFilePath) {

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

pacman::Cell pacman::Maze::charToCell(char & c) {
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
