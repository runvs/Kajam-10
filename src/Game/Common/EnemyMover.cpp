#include "EnemyMover.hpp"
#include "EnemyState.hpp"

void EnemyMoverBase::move(EnemyState& state, float elapsed)
{
    jt::Vector2 const offset { 0, Game::GameProperties::scrollSpeed() * elapsed };
    state.position += offset;
    state._positionBase += offset;
    doMove(state, elapsed);
}

void EnemyMoverStandStill::doMove(EnemyState& state, float elapsed) { }

void EnemyMoverSine::doMove(EnemyState& state, float elapsed)
{
    float posxOffset = (sin(state._age * 2.0f + state._moveDelay) * 25.0f);
    state.position = state._positionBase + jt::Vector2 { posxOffset, 0.0f };
}
