﻿#ifndef ENEMY_MOVER_HPP_GUARD
#define ENEMY_MOVER_HPP_GUARD

struct EnemyState;

class EnemyMoverBase {
public:
    EnemyMoverBase() = default;
    virtual ~EnemyMoverBase() = default;

    void move(EnemyState& state, float elapsed);

private:
    virtual void doMove(EnemyState& state, float elapsed) = 0;
};

class EnemyMoverStandStill : public EnemyMoverBase {
    void doMove(EnemyState& state, float elapsed) override;
};

class EnemyMoverSine : public EnemyMoverBase {
    void doMove(EnemyState& state, float elapsed) override;
};

#endif
