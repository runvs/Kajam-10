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
    const float posOffsetX = (sin(state._age * 2.0f + state._moveDelay) * 25.0f);
    state.position = state._positionBase + jt::Vector2 { posOffsetX, 0.0f };
}

void EnemyMoverCircle::doMove(EnemyState& state, float elapsed)
{
    if (m_sequenceId == 0) {
        if (state.position.y() >= 100) {
            m_sequenceId = 1;
            m_timer = 0.0f;
        }
    } else if (m_sequenceId == 1) {
        m_timer += elapsed;
        // remove normal movement
        jt::Vector2 const normalMoveOffset { 0, Game::GameProperties::scrollSpeed() * elapsed };
        state.position -= normalMoveOffset;
        state._positionBase -= normalMoveOffset;

        const float posOffsetX = (sin(m_timer * 1.85f) * 48.0f);
        const float posOffsetY = ((cos(m_timer * 1.85f) - 1) * 48.0f);
        state.position = state._positionBase + jt::Vector2 { posOffsetX, posOffsetY };
    }
}
