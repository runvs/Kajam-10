#ifndef SHOTPATTERN_GUARD_HPP
#define SHOTPATTERN_GUARD_HPP

namespace Shots {
struct ShotPattern {
    static int SINGLE() { return 1; }
    static int TRIPLE_NARROW() { return 2; }
    static int TRIPLE_WIDE() { return 4; }
    static int SIDE() { return 8; }
    static int BEHIND() { return 16; }
    static int level1() { return Shots::ShotPattern::SINGLE(); }
    static int level2() { return Shots::ShotPattern::TRIPLE_WIDE(); }
    static int level3() { return Shots::ShotPattern::TRIPLE_NARROW(); }
    static int level4()
    {
        return Shots::ShotPattern::TRIPLE_NARROW() | Shots::ShotPattern::BEHIND();
    }
    static int level5()
    {
        return Shots::ShotPattern::TRIPLE_NARROW() | Shots::ShotPattern::BEHIND()
            | Shots::ShotPattern::SIDE();
    }
    static int level6()
    {
        return Shots::ShotPattern::TRIPLE_NARROW() | Shots::ShotPattern::TRIPLE_WIDE()
            | Shots::ShotPattern::BEHIND() | Shots::ShotPattern::SIDE();
    }
};
} // namespace Shots

#endif
