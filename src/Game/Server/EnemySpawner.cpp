#include "EnemySpawner.hpp"
#include "EnemyAI.hpp"
#include "Random.hpp"

#include <iostream>

EnemySpawner::EnemySpawner(std::vector<EnemyState>& enemies)
    : m_enemies(enemies)
    , m_timer(0.0f)
    , m_timerMax(7.5f)
{
}

void EnemySpawner::update(float elapsed)
{
    m_timer += elapsed;
    auto const maxTimer = getMaxTimer();
    if (m_timer >= m_timerMax) {
        m_timer -= m_timerMax;
        spawnEnemyGroup();
    }
}

void EnemySpawner::setActivePlayerCount(int activePlayers) { m_activePlayerCount = activePlayers; }

void EnemySpawner::setCurrentlyAliveEnemies(int currentlyAlive)
{
    m_enemyNumberCurrent = currentlyAlive;
}

void EnemySpawner::setDifficulty(float difficulty) { m_difficulty = difficulty; }

int EnemySpawner::getEnemyNumberTarget()
{
    return 5 + m_activePlayerCount * Game::GameProperties::spawnerEnemiesPerPlayer();
}

int EnemySpawner::getGroupSize() { return 2; }

float EnemySpawner::getMaxTimer()
{
    float exponent = 1.0f;
    float currentEnemies = static_cast<float>(m_enemyNumberCurrent);
    float targetEnemies = static_cast<float>(getEnemyNumberTarget());
    float ret = std::pow(currentEnemies / targetEnemies, exponent) * m_timerMax;
    return ret;
}

EnemyState EnemySpawner::createBaseEnemy(float basePosX, int i)
{
    EnemyState enemy;
    enemy.EnemyState::position = jt::Vector2 { basePosX, -100.0f + i * 24.0f };
    enemy.EnemyState::_positionBase = enemy.EnemyState::position;
    enemy.EnemyState::_moveDelay = i * 0.5f;
    enemy._health = static_cast<int>(Game::GameProperties::enemyDefaultHealth() * m_difficulty);
    return enemy;
}

void EnemySpawner::spawnEnemyStandStill(float basePosX, int i)
{
    auto enemy = createBaseEnemy(basePosX, i);
    enemy._mover = std::make_shared<EnemyAIIdle>();
    m_enemies.emplace_back(std::move(enemy));
}

void EnemySpawner::spawnEnemySine(float basePosX, int i)
{
    auto enemy = createBaseEnemy(basePosX, i);
    enemy._mover = std::make_shared<EnemyAISine>();
    m_enemies.emplace_back(std::move(enemy));
}

void EnemySpawner::spawnEnemyCircle(float basePosX, int i)
{
    if (i >= 3) {
        return;
    }
    auto enemy = createBaseEnemy(basePosX, i);
    enemy._mover = std::make_shared<EnemyAICircle>();
    m_enemies.emplace_back(std::move(enemy));
}

void EnemySpawner::SpawnEnemy(int groupType, float basePosX, int i)
{
    if (groupType == 0) {
        spawnEnemyStandStill(basePosX, i);
    } else if (groupType == 1) {
        spawnEnemySine(basePosX, i);
    } else {
        spawnEnemyCircle(basePosX, i);
    }
}

void EnemySpawner::spawnEnemyGroup()
{
    auto const count = getGroupSize() + jt::Random::getInt(1, 2);
    int groupType = jt::Random::getInt(0, 2);
    int margin = 40;
    float basePosX = jt::Random::getFloat(margin,
        Game::GameProperties::displayScreenSize().x() - margin
            - Game::GameProperties::playerSizeInPixel());
    for (int i = 0; i != count; ++i) {
        SpawnEnemy(groupType, basePosX, i);
    }
}
