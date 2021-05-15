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
    void spawnGroupIdleVertical(float basePosX) const;
    void spawnGroupIdleHorizontal(float basePosX) const;
    void spawnGroupSine(float basePosX) const;
    void spawnGroupCircle(float basePosX) const;
    void spawnGroupMine(float basePosX) const;

    void spawnEnemyGroup();
};

#endif
