#include <fstream>
#include <string>
#include <iostream>

#include "../include/Cell.h"
#include "../include/Maze.h"

pacman::Maze::Maze(std::shared_ptr<std::vector<std::vector<Cell>>> mazeMatrix, long cellHeight, long cellWidth)
    : mazeMatrix(mazeMatrix), cellHeight(cellHeight), cellWidth(cellWidth) {}

long pacman::Maze::getCellHeight() {
    return cellHeight;
}

long pacman::Maze::getCellWidth() {
    return cellWidth;
}

std::shared_ptr<std::vector<std::vector<pacman::Cell>>> pacman::Maze::getMazeMatrix() {
    return mazeMatrix;
}
