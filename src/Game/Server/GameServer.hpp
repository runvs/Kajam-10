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

    std::map<int, jt::CircularBuffer<std::size_t, 20>> receivedMessageIdsMap;

    std::vector<ShotState> shots;
    std::vector<EnemyState> enemies;

    EnemySpawner spawner { enemies };

    int score { 0 };
    float elapsed = 0.0f;
    void removeInactivePlayers();
    jt::Vector2 getRandomNewPlayerPosition() const;
    void createNewPlayerIfNotKnownToServer(int currentPlayerId);
    bool performShotEnemyCollision(
        std::vector<ShotState>::value_type& s, std::vector<EnemyState>::value_type& enemy);
    bool GameServer::checkForDuplicatedMessages(
        std::map<int, jt::CircularBuffer<std::size_t, 20>> receivedMessageIdsMap,
        int currentPlayerId, std::size_t const messageId);
    bool performShotPlayerCollision(PlayerState& player, ShotState& shot);
    void performPlayerEnemyCollision(PlayerState& player, EnemyState& enemy);
};

#endif
