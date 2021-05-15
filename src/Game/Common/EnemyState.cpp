#include "EnemyState.hpp"
#include "Conversions.hpp"
#include "EnemyAI.hpp"
#include "ShotState.hpp"

sf::Packet& operator<<(sf::Packet& packet, EnemyState& enemyState)
{
    return packet << enemyState.position;
}

sf::Packet& operator<<(sf::Packet& packet, EnemyState const& enemyState)
{
    return packet << enemyState.position;
}

sf::Packet& operator>>(sf::Packet& packet, EnemyState& enemyState)
{
    return packet >> enemyState.position;
}

void enemyTakeDamage(EnemyState& enemyState, ShotState& shotState)
{
    enemyState._health -= shotState._damage;
    enemyState._alive = enemyState._health > 0;
}

void updateEnemyState(EnemyState& enemyState, std::vector<ShotState>& shots, float elapsed)
{
    enemyState._age += elapsed;
    if (enemyState._ai) {
        enemyState._ai->update(enemyState, shots, elapsed);
    }

    if (enemyState.position.y() >= Game::GameProperties::displayScreenSize().y() + 24) {
        enemyState._alive = false;
    }
    if (enemyState._age >= 25) {
        enemyState._alive = false;
    }
}
