#include "EnemySpawner.hpp"
#include "EnemyAI.hpp"
#include "Random.hpp"
#include <functional>
#include <iostream>
#include <vector>

EnemySpawner::EnemySpawner(std::vector<EnemyState>& enemies)
    : m_enemies { enemies }
    , m_timer { 0.0f }
    , m_timerMax { 4.5f }
    , m_difficulty { 1.0f }
    , m_activePlayerCount(0)
    , m_sah { 20.0f, 3.5f }
{
    fillSpawnFunctions();
}

void EnemySpawner::update(float elapsed)
{
    if (m_activePlayerCount == 0) {
        // do not spawn enemies if no players are present
        return;
    }
    m_timer += elapsed;
    m_sah.update(elapsed);

    auto const maxTimer = getMaxTimer();
    if (m_timer >= maxTimer) {
        m_timer -= maxTimer;
        spawnEnemyGroup();
    }
}

void EnemySpawner::setActivePlayerCount(int activePlayers) { m_activePlayerCount = activePlayers; }

void EnemySpawner::setDifficulty(float difficulty) { m_difficulty = difficulty; }

int EnemySpawner::getEnemyNumberTarget() const
{
    return 8 + m_activePlayerCount * Game::GameProperties::spawnerEnemiesPerPlayer();
}

int EnemySpawner::getGroupSize() { return 2; }

float EnemySpawner::getMaxTimer() const
{
    float const exponent = 1.2f;
    auto const currentEnemies = static_cast<float>(m_enemies.size());
    auto const targetEnemies = static_cast<float>(getEnemyNumberTarget() * m_difficulty);
    return std::pow(currentEnemies / targetEnemies, exponent) * m_timerMax;
}

void EnemySpawner::fillSpawnFunctions()
{
    m_spawnFunctions.emplace_back([this](auto const value) { spawnGroupIdleVertical(value); });
    m_spawnFunctions.emplace_back([this](auto const value) { spawnGroupIdleHorizontal(value); });
    m_spawnFunctions.emplace_back([this](auto const value) { spawnGroupSine(value); });
    m_spawnFunctions.emplace_back([this](auto const value) { spawnGroupCircle(value); });
    m_spawnFunctions.emplace_back([this](auto const value) { spawnGroupMine(value); });
}

EnemyState EnemySpawner::createBaseEnemy(float basePosX, int i) const
{
    EnemyState enemy;
    enemy.position = jt::Vector2 { basePosX, -24.0f - i * 24.0f };
    enemy._positionBase = enemy.EnemyState::position;
    enemy._moveDelay = -i * 0.5f;
    enemy._health = static_cast<int>(
        static_cast<float>(Game::GameProperties::enemyDefaultHealth()) * m_difficulty);
    return enemy;
}

void EnemySpawner::spawnGroupIdleVertical(float basePosX) const
{
    int const max = jt::Random::getInt(1, 2);
    for (int i = 0; i != max; ++i) {
        auto enemy = createBaseEnemy(basePosX, i);
        enemy._ai = std::make_shared<EnemyAIIdle>();
        m_enemies.emplace_back(std::move(enemy));
    }
}

void EnemySpawner::spawnGroupIdleHorizontal(float basePosX) const
{
    int const max = jt::Random::getInt(2, 4);
    for (int i = 0; i != max; ++i) {
        auto enemy = createBaseEnemy(basePosX, i);
        enemy.position = jt::Vector2 { basePosX + i * 24, -32 };
        enemy._ai = std::make_shared<EnemyAIIdle>();
        m_enemies.emplace_back(std::move(enemy));
    }
}

void EnemySpawner::spawnGroupSine(float basePosX) const
{
    int const max = jt::Random::getInt(4, 5);
    for (int i = 0; i != max; ++i) {
        auto enemy = createBaseEnemy(basePosX, i);
        enemy._ai = std::make_shared<EnemyAISine>();
        m_enemies.emplace_back(std::move(enemy));
    }
}

void EnemySpawner::spawnGroupCircle(float basePosX) const
{
    float const circlePosY = jt::Random::getFloat(80, 150);
    for (int i = 0; i != 3; ++i) {
        auto enemy = createBaseEnemy(basePosX, i);
        enemy._ai = std::make_shared<EnemyAICircle>(circlePosY);
        m_enemies.emplace_back(std::move(enemy));
    }
}

void EnemySpawner::spawnGroupMine(float basePosX) const
{
    auto enemy = createBaseEnemy(basePosX, 0);
    enemy._ai = std::make_shared<EnemyAIMine>(jt::Random::getFloat(80, 150));
    m_enemies.emplace_back(std::move(enemy));
}

float getBasePosX()
{
    float const margin = 40.0f;
    return jt::Random::getFloat(margin,
        Game::GameProperties::displayScreenSize().x() - margin
            - Game::GameProperties::playerSizeInPixel());
}

void EnemySpawner::spawnEnemyGroup()
{
    int const groupType
        = static_cast<int>(m_sah.getFloat() * static_cast<float>(m_spawnFunctions.size()));
    m_spawnFunctions.at(groupType)(getBasePosX());
}
