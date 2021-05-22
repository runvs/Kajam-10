#ifndef SHOTPATTERN_GUARD_HPP
#define SHOTPATTERN_GUARD_HPP

namespace Shots {
struct ShotPattern {
    static int SINGLE() { return 1; }
    static int TRIPLE_NARROW() { return 2; }
    static int TRIPLE_WIDE() { return 4; }
    static int SIDE() { return 8; }
    static int BEHIND() { return 16; };
};
} // namespace Shots

#endif
