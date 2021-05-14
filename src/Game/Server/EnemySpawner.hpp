#ifndef ENEMYSPAWNER_HPP_GUARD
#define ENEMYSPAWNER_HPP_GUARD

#include "EnemyState.hpp"
#include <vector>

class EnemySpawner {
public:
    explicit EnemySpawner(std::vector<EnemyState>& enemies);

    void update(float elapsed);

private:
    std::vector<EnemyState>& m_enemies;
    float m_timer;
    float m_timerMax;

    int getGroupSize();
    float getMaxTimer();
    EnemyState createBaseEnemy(float basePosX, int i);
    void spawnEnemyStandStill(float basePosX, int i);
    void spawnEnemySine(float basePosX, int i);
    void SpawnEnemy(int groupType, float basePosX, int i);
    void spawnEnemyGroup();
};

#endif
