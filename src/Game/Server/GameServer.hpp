#ifndef GAMESERVER_HPP_GUARD
#define GAMESERVER_HPP_GUARD
#include "CircularBuffer.hpp"
#include "EnemySpawner.hpp"
#include "NetworkServer.hpp"

class GameServer {
public:
    void update();

private:
    NetworkServer m_networkServer;

    PlayerMap m_playerStates;
    std::map<int, std::size_t> playerPredictionId;

    std::map<int, jt::CircularBuffer<std::size_t, 20>> m_receivedMessageIdsMap;

    std::vector<ShotState> shots;
    std::vector<EnemyState> enemies;

    EnemySpawner spawner { enemies };

    int m_score { 0 };
    float elapsed = 0.0f;

    void removeInactivePlayers();
    jt::Vector2 getRandomNewPlayerPosition() const;
    void createNewPlayerIfNotKnownToServer(int currentPlayerId);
    void performShotEnemyCollision(ShotState& s, EnemyState& enemy);
    bool checkForDuplicatedMessages(int currentPlayerId, std::size_t const messageId);
    void performShotPlayerCollision(PlayerState& player, ShotState& shot);
    void performPlayerEnemyCollision(PlayerState& player, EnemyState& enemy);
};

#endif
