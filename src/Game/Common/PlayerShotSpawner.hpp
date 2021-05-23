#ifndef PLAYERSHOTSPAWNER_GUARD_HPP
#define PLAYERSHOTSPAWNER_GUARD_HPP

#include "PlayerState.hpp"
#include "ShotState.hpp"
#include <vector>

class PlayerShotSpawner {
public:
    static std::vector<ShotState> createShotFromPattern(
        int pattern, jt::Vector2 const& playerPosition);

private:
    static void createShotSingle(
        std::vector<ShotState>& shotStates, jt::Vector2 const& shotPosition);
    static void createShotTripleNarrow(
        std::vector<ShotState>& shotStates, jt::Vector2 const& shotPosition);
    static void createShotTripleWide(
        std::vector<ShotState>& shotStates, jt::Vector2 const& shotPosition);
    static void createShotSide(std::vector<ShotState>& shotStates, jt::Vector2 const& shotPosition);
    static void createShotBehind(
        std::vector<ShotState>& shotStates, jt::Vector2 const& shotPosition);
};

#endif // PLAYERSHOTSPAWNER_GUARD_HPP
