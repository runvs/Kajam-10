#ifndef ENEMY_AI_HPP_GUARD
#define ENEMY_AI_HPP_GUARD
#include "EnemyState.hpp"
#include "Random.hpp"
#include "ShotState.hpp"
class EnemyAI {
public:
    EnemyAI() = default;
    ~EnemyAI() = default;
    void update(EnemyState& state, std::vector<ShotState>& shots, float elapsed);

private:
    virtual void doUpdate(EnemyState& state, std::vector<ShotState>& shots, float elapsed) = 0;
};

class EnemyAIIdle : public EnemyAI {
    void doUpdate(EnemyState& state, std::vector<ShotState>& shots, float elapsed) override;
};

class EnemyAISine : public EnemyAI {
    void doUpdate(EnemyState& state, std::vector<ShotState>& shots, float elapsed) override;
};

class EnemyAICircle : public EnemyAI {
public:
    EnemyAICircle(float circlePosition)
        : m_circlePositionY(circlePosition)
    {
        m_shotTimer = jt::Random::getFloat(0.0f, 3.5f);
    }

private:
    void doUpdate(EnemyState& state, std::vector<ShotState>& shots, float elapsed) override;
    int m_sequenceId { 0 };
    float m_circlePositionY { 100 };
    float m_timer { 0.0f };
    float m_shotTimer { 0.0f };
};
#endif
