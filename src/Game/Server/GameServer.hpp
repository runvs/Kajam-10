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

    std::vector<ShotState> m_shots;
    std::vector<EnemyState> m_enemies;

    EnemySpawner m_enemySpawner { m_enemies };
    std::vector<PowerupState> m_powerups;

    int m_score { 0 };
    float m_elapsed = 0.0f;
    std::vector<int> m_playersToDisconnect {};

    void removeInactivePlayers();
    void removeDisconnectedPlayers();

    jt::Vector2 getRandomNewPlayerPosition() const;
    void createNewPlayerIfNotKnownToServer(int currentPlayerId);

    bool checkForDuplicatedMessages(int currentPlayerId, std::size_t const messageId);
    void sortIncomingPayloadsForPlayer(std::vector<PayloadClient2Server> dataForPlayer) const;
    bool handleSinglePayloadForSinglePlayer(
        int currentPlayerId, PayloadClient2Server const& payload);
    void handleAllPayloadForSinglePlayer(int currentPlayerId);
    void handleAllPayloadsForAllPlayers();

    void sendSinglePayloadToPlayer(std::pair<int, PlayerState> const& kvp);
    void sendPayloadToPlayers();

    void performShotEnemyCollision(ShotState& shot, EnemyState& enemy);
    void performShotPlayerCollision(PlayerState& player, ShotState& shot);
    void performPlayerEnemyCollision(EnemyState& enemy, PlayerState& player);
    void performAllPlayerEnemyCollision();

    ShotState createPlayerShot(jt::Vector2 const& playerPosition) const;
    void handlePlayerShooting(int currentPlayerId, PayloadClient2Server payload);

    void handleEnemySpawning();

    void updateAllEnemies();

    void updateAllShots();
    void handleSingleShotCollision(ShotState& s);
    void handleAllShotCollisions();

    void handlePowerupEffect(PowerupState& powerup, PlayerState& player);
    bool performPlayerPowerupCollision(
        std::vector<PowerupState>::value_type& powerup, PlayerState& player);

    void updateAllPowerups();

    void removeDeadEnemies();
    void removeDeadShots();
    void removeDeadPowerups();
};

#endif
