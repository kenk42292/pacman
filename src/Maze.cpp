#include <cmath>
#include <fstream>
#include <string>
#include <iostream>

#include "../include/Maze.h"

pacman::Maze::Maze(std::shared_ptr<std::vector<std::vector<Cell>>> mazeMatrix, long cellHeight, long cellWidth)
    : mazeMatrix(mazeMatrix), cellHeight(cellHeight), cellWidth(cellWidth) {}

long pacman::Maze::getCellHeight() {
    return cellHeight;
}

long pacman::Maze::getCellWidth() {
    return cellWidth;
}

std::shared_ptr<std::vector<std::vector<pacman::Maze::Cell>>> pacman::Maze::getMazeMatrix() {
    return mazeMatrix;
}long distance(std::pair<long, long> a, std::pair<long, long> b);

// TODO: Verify for rounding...
bool pacman::Maze::isValid(std::pair<long, long> location) {
    int numRows = mazeMatrix->size(), numColumns = mazeMatrix->at(0).size();
    long upperLeftY = location.first - cellHeight/2;
    long upperLeftX = location.second - cellWidth/2;
    long lowerRightY = location.first + cellHeight/2-1;
    long lowerRightX = location.second + cellHeight/2-1;
    int i1 = upperLeftY/cellHeight, j1 = upperLeftX/cellWidth, i2 = lowerRightY/cellHeight, j2 = lowerRightX/cellWidth;
    return 0<=i1 && i1<numRows && 0<=i2 && i2<numRows
            && 0<=j1 && j1<numColumns && 0<=j2 && j2<numColumns
            && mazeMatrix->at(i1).at(j1) != Cell::kBorder
            && mazeMatrix->at(i1).at(j2) != Cell::kBorder
            && mazeMatrix->at(i2).at(j1) != Cell::kBorder
            && mazeMatrix->at(i2).at(j2) != Cell::kBorder;
}

bool pacman::Maze::isCellValid(std::pair<int, int> coordinates) {
    int i = coordinates.first, j = coordinates.second;
    return mazeMatrix->at(i).at(j) != Cell::kBorder;
}

bool pacman::Maze::isCellAligned(std::pair<long, long> location) {
    long y = location.first, x = location.second;
    return (y-cellHeight/2) % cellHeight == 0 && (x-cellWidth/2) % cellHeight == 0;
}

bool pacman::Maze::isAxisAligned(std::pair<long, long> location) {
    long y = location.first, x = location.second;
    return (y-cellHeight/2) % cellHeight == 0 || (x-cellWidth/2) % cellHeight == 0;
}

// TODO: Currently the manhattan distance
long pacman::Maze::distance(std::pair<long, long> a, std::pair<long, long> b) {
    return std::abs(a.first-b.first) + std::abs(a.second-b.second);
}

bool pacman::Maze::clearPellet(std::pair<long, long> agentCenter) {
    int i = agentCenter.first/cellHeight;
    int j = agentCenter.second/cellWidth;
    if (mazeMatrix->at(i).at(j) == Maze::Cell::kPellet) {
        mazeMatrix->at(i).at(j) = Maze::Cell::kEmpty;
        return true;
    }
    return false;
}
