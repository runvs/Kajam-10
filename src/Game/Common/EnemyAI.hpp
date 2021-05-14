#ifndef ENEMY_AI_HPP_GUARD
#define ENEMY_AI_HPP_GUARD
#include "EnemyState.hpp"
#include "ShotState.hpp"
class EnemyAI {
public:
    EnemyAI() = default;
    ~EnemyAI() = default;
    void move(EnemyState& state, std::vector<ShotState>& shots, float elapsed);

private:
    virtual void doMove(EnemyState& state, std::vector<ShotState>& shots, float elapsed) = 0;
};

class EnemyAIIdle : public EnemyAI {
    void doMove(EnemyState& state, std::vector<ShotState>& shots, float elapsed) override;
};

class EnemyAISine : public EnemyAI {
    void doMove(EnemyState& state, std::vector<ShotState>& shots, float elapsed) override;
};

class EnemyAICircle : public EnemyAI {
    void doMove(EnemyState& state, std::vector<ShotState>& shots, float elapsed) override;
    int m_sequenceId { 0 };
    float m_timer { 0.0f };
};
#endif
