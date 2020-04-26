#ifndef PACMAN_UTILS_H_
#define PACMAN_UTILS_H_

#include <functional>

namespace pacman {

/** A bunch of static utilities that are useful to the pacman program. */
class Utils {
public:
    /** Rounded distance between two locations specified by std::pair<long, long>. */
    static long distance(std::pair<long, long> a, std::pair<long, long> b);
private:
    // Non-instantiable class.
    Utils();
};

} // namespace pacman

#endif // PACMAN_UTILS_H_
