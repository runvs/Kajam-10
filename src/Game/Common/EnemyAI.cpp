#include "EnemyAI.hpp"
#include "EnemyState.hpp"

void EnemyAI::move(EnemyState& state, std::vector<ShotState>& shots, float elapsed)
{
    jt::Vector2 const offset { 0, Game::GameProperties::scrollSpeed() * elapsed };
    state.position += offset;
    state._positionBase += offset;
    doMove(state, shots, elapsed);
}

void EnemyAIIdle::doMove(EnemyState& state, std::vector<ShotState>& shots, float elapsed) { }

void EnemyAISine::doMove(EnemyState& state, std::vector<ShotState>& shots, float elapsed)
{
    const float posOffsetX
        = static_cast<float>((sin(state._age * 2.0f + state._moveDelay) * 25.0f));
    state.position = state._positionBase + jt::Vector2 { posOffsetX, 0.0f };
}

void EnemyAICircle::doMove(EnemyState& state, std::vector<ShotState>& shots, float elapsed)
{
    if (m_sequenceId == 0) {
        if (state.position.y() >= m_circlePositionY) {
            m_sequenceId = 1;
            m_timer = 0.0f;
        }
    } else if (m_sequenceId == 1) {
        m_timer += elapsed;
        // remove normal movement
        jt::Vector2 const normalMoveOffset { 0, Game::GameProperties::scrollSpeed() * elapsed };
        state.position -= normalMoveOffset;
        state._positionBase -= normalMoveOffset;

        const float posOffsetX = static_cast<float>((sin(m_timer * 1.85f) * 48.0f));
        const float posOffsetY = static_cast<float>(((cos(m_timer * 1.85f) - 1) * 48.0f));
        state.position = state._positionBase + jt::Vector2 { posOffsetX, posOffsetY };
    }
}
