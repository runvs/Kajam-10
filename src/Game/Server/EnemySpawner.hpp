#ifndef ENEMYSPAWNER_HPP_GUARD
#define ENEMYSPAWNER_HPP_GUARD

#include "EnemyState.hpp"
#include "Random.hpp"
#include <functional>
#include <vector>

class EnemySpawner {
public:
    explicit EnemySpawner(std::vector<EnemyState>& enemies);

    void update(float elapsed);

    void setActivePlayerCount(int activePlayers);
    void setCurrentlyAliveEnemies(int currentlyAlive);
    void setDifficulty(float difficulty = 1.0f);

private:
    std::vector<EnemyState>& m_enemies;
    float m_timer;
    float m_timerMax;

    int m_enemyNumberCurrent;
    float m_difficulty;
    int m_activePlayerCount;
    jt::SampleAndHold m_sah;
    std::vector<std::function<void(float)>> m_spawnFunctions;

    void fillSpawnFunctions();
    int getEnemyNumberTarget() const;
    static int getGroupSize();
    float getMaxTimer() const;
    EnemyState createBaseEnemy(float basePosX, int i) const;
    void spawnEnemyGroupIdleVertical(float basePosX) const;
    void spawnEnemyGroupIdleHorizontal(float basePosX) const;
    void spawnEnemyGroupSine(float basePosX) const;
    void spawnEnemyGroupCircle(float basePosX) const;

    void SpawnEnemy(int groupType, float basePosX);
    void spawnEnemyGroup();
};

#endif
