
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../include/Cell.h"
#include "../include/MazeUtils.h"

std::vector<std::vector<pacman::Cell>> pacman::mazeutils::readMazeMatrix(std::string mazeFilePath) {
    
    std::ifstream inputFileStream(mazeFilePath);
    if (!inputFileStream.is_open()) {
        std::cout << "Failed to read in input file[" << "res/maze1.txt" << '\n';
        exit(1);
    }

    std::vector<std::vector<pacman::Cell>> mazeMatrix;

    std::string line;
    char c;
    while (std::getline(inputFileStream, line)) {
        std::vector<pacman::Cell> row;
        for (auto it=line.begin() ; it != line.end(); ++it) {
            row.push_back(pacman::mazeutils::charToCell(*it));
        }
        mazeMatrix.push_back(row);
    }

    inputFileStream.close();

    return mazeMatrix;
}

pacman::Cell pacman::mazeutils::charToCell(char & c) {
    switch (c) {
        case '|':
        case '+':
        case '-':
        case '_':
            return pacman::Cell::kBorder;
        case '.':
            return pacman::Cell::kPellet;
        case ' ':
            return pacman::Cell::kEmpty;
        default:
            throw std::invalid_argument("Invalid char[" + std::string(1, c) + "] to be converted to cell.");
    }
}

