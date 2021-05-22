#include "PlayerShotSpawner.hpp"
#include "ShotState.hpp"
#include <array>
#include <memory>

std::vector<ShotState> PlayerShotSpawner::createShotFromPattern(
    int pattern, jt::Vector2 const& playerPosition)
{
    auto const shotOffset
        = jt::Vector2 { static_cast<float>(Game::GameProperties::playerSizeInPixel()) / 2.0f - 4.0f,
              0.0f };
    auto const shotPosition = playerPosition + shotOffset;
    // auto shotState = ShotState { shotPosition, shotDirection };
    // return shotState;

    auto shotStates = std::vector<ShotState> {};

    if ((pattern & Shots::ShotPattern::SINGLE()) == Shots::ShotPattern::SINGLE()) {
        createShotSingle(shotStates, shotPosition);
    }

    if ((pattern & Shots::ShotPattern::TRIPLE_NARROW()) == Shots::ShotPattern::TRIPLE_NARROW()) {
        createShotTripleNarrow(shotStates, shotPosition);
    }

    if ((pattern & Shots::ShotPattern::TRIPLE_WIDE()) == Shots::ShotPattern::TRIPLE_WIDE()) {
        createShotTripleWide(shotStates, shotPosition);
    }

    if ((pattern & Shots::ShotPattern::SIDE()) == Shots::ShotPattern::SIDE()) {
        createShotSide(shotStates, shotPosition);
    }

    if ((pattern & Shots::ShotPattern::BEHIND()) == Shots::ShotPattern::BEHIND()) {
        auto shotBehindOffset = jt::Vector2 { shotOffset.x(),
            static_cast<float>(Game::GameProperties::playerSizeInPixel()) / 2.0f };
        auto shotBehindPosition = playerPosition + shotBehindOffset;
        createShotBehind(shotStates, shotBehindPosition);
    }

    return shotStates;
}

void PlayerShotSpawner::createShotSingle(
    std::vector<ShotState>& shotStates, jt::Vector2 const& shotPosition)
{
    auto const shotDirection = getShotJitterDirection(
        jt::Vector2 { 0, -1 }, Game::GameProperties::shotBaseJitterAmount());

    auto shotState = ShotState { shotPosition, shotDirection };
    shotStates.emplace_back(shotState);
}

void PlayerShotSpawner::createShotTripleNarrow(
    std::vector<ShotState>& shotStates, jt::Vector2 const& shotPosition)
{
    std::array<jt::Vector2, 3> baseDirections = { { { -0.1f, -1 }, { 0, -1 }, { 0.1f, -1 } } };

    for (auto& b : baseDirections) {
        auto const shotDirection
            = getShotJitterDirection(b, Game::GameProperties::shotBaseJitterAmount());

        auto shotState = ShotState { shotPosition, shotDirection };
        shotStates.emplace_back(shotState);
    }
}

void PlayerShotSpawner::createShotTripleWide(
    std::vector<ShotState>& shotStates, jt::Vector2 const& shotPosition)
{
    std::array<jt::Vector2, 3> baseDirections = { { { -0.5f, -1 }, { 0, -1 }, { 0.5f, -1 } } };

    for (auto& b : baseDirections) {
        auto const shotDirection
            = getShotJitterDirection(b, Game::GameProperties::shotBaseJitterAmount());

        auto shotState = ShotState { shotPosition, shotDirection };
        shotStates.emplace_back(shotState);
    }
}

void PlayerShotSpawner::createShotSide(
    std::vector<ShotState>& shotStates, jt::Vector2 const& shotPosition)
{
    std::array<jt::Vector2, 2> baseDirections = { { { -1, 0 }, { 1, 0 } } };

    for (auto& b : baseDirections) {
        auto const shotDirection
            = getShotJitterDirection(b, Game::GameProperties::shotBaseJitterAmount());

        auto shotState = ShotState { shotPosition, shotDirection };
        shotStates.emplace_back(shotState);
    }
}

void PlayerShotSpawner::createShotBehind(
    std::vector<ShotState>& shotStates, jt::Vector2 const& shotPosition)
{
    auto const shotDirection = getShotJitterDirection(
        jt::Vector2 { 0, 1 }, Game::GameProperties::shotBaseJitterAmount());

    auto shotState = ShotState { shotPosition, shotDirection };
    shotStates.emplace_back(shotState);
}
