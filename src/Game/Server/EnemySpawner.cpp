#include "EnemySpawner.hpp"
#include "EnemyMover.hpp"
#include "Random.hpp"

#include <iostream>

EnemySpawner::EnemySpawner(std::vector<EnemyState>& enemies)
    : m_enemies(enemies)
    , m_timer(0.0f)
    , m_timerMax(8.0f)
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
    return m_activePlayerCount * Game::GameProperties::spawnerEnemiesPerPlayer();
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
    enemy._mover = std::make_shared<EnemyMoverStandStill>();
    m_enemies.emplace_back(std::move(enemy));
}

void EnemySpawner::spawnEnemySine(float basePosX, int i)
{
    auto enemy = createBaseEnemy(basePosX, i);
    enemy._mover = std::make_shared<EnemyMoverSine>();
    m_enemies.emplace_back(std::move(enemy));
}

void EnemySpawner::SpawnEnemy(int groupType, float basePosX, int i)
{
    if (groupType == 0) {
        spawnEnemyStandStill(basePosX, i);
    } else {
        spawnEnemySine(basePosX, i);
    }
}

void EnemySpawner::spawnEnemyGroup()
{
    auto const count = getGroupSize() + jt::Random::getInt(1, 3);
    int groupType = jt::Random::getInt(0, 1);
    int margin = 40;
    float basePosX = jt::Random::getFloat(margin,
        Game::GameProperties::displayScreenSize().x() - margin
            - Game::GameProperties::playerSizeInPixel());
    for (int i = 0; i != count; ++i) {
        SpawnEnemy(groupType, basePosX, i);
    }
}
