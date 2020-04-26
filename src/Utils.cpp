
#include <cmath>

#include "../include/Utils.h"

long pacman::Utils::distance(std::pair<long, long> a, std::pair<long, long> b) {
  return sqrt(pow(a.first - b.first, 2) + pow(b.second - b.second, 2));
}
