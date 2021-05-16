#include "CircularBuffer.hpp"
#include "EnemySpawner.hpp"
#include "EnemyState.hpp"
#include "GameProperties.hpp"
#include "NetworkServer.hpp"
#include "Payloads.hpp"
#include "PlayerState.hpp"
#include "ShotState.hpp"
#include "SystemHelper.hpp"
#include "common.hpp"
#include <iostream>

void removeInactivePlayers(PlayerMap& playerStates, std::vector<int> playerIds)
{
    if (playerIds.size() != playerStates.size()) {
        jt::SystemHelper::erase_if(playerStates, [&playerIds](auto const& kvp) {
            return std::find(playerIds.begin(), playerIds.end(), kvp.first) == playerIds.end();
        });
    }
}

void createNewPlayerIfNotKnownToServer(
    PlayerMap playerStates, std::vector<int>::value_type currentPlayerId)
{
    if (playerStates.count(currentPlayerId) == 0) {
        playerStates[currentPlayerId].position.x() = 0;
        playerStates[currentPlayerId].position.y() = 0;
    }
}

bool checkForDuplicatedMessages(
    std::map<int, jt::CircularBuffer<std::size_t, 20>> receivedMessageIdsMap, int currentPlayerId,
    std::size_t const messageId)
{
    if (receivedMessageIdsMap[currentPlayerId].contains(messageId)) {
        std::cout << "skip duplicated message for player " << currentPlayerId << std::endl;
        return true;
    }
    receivedMessageIdsMap[currentPlayerId].push(messageId);
    return false;
}

void performShotEnemyCollision(
    std::vector<ShotState>::value_type& s, std::vector<EnemyState>::value_type& enemy)
{
    auto const enemyHalfSize = Game::GameProperties::enemyHalfSize();
    auto const shotHalfSize = Game::GameProperties::shotHalfSize();
    auto const centerPositionEnemy = enemy.position + enemyHalfSize;
    auto const centerPositionShot = s.position + shotHalfSize;
    auto const diff = centerPositionShot - centerPositionEnemy;
    auto const lSquared = jt::MathHelper::lengthSquared(diff);

    if (lSquared
        < (enemyHalfSize.x() + shotHalfSize.x()) * (enemyHalfSize.y() + shotHalfSize.y())) {
        s._alive = false;
        enemyTakeDamage(enemy, s);
    }
}

void performShotPlayerCollision(PlayerState& player, ShotState& s)
{
    auto const playerHalfSize = jt::Vector2 { Game::GameProperties::playerSizeInPixel() / 2.0f,
        Game::GameProperties::playerSizeInPixel() / 2.0f };
    auto const shotHalfSize = Game::GameProperties::shotHalfSize();
    auto const centerPositionPlayer = player.position + playerHalfSize;
    auto const centerPositionShot = s.position + shotHalfSize;
    auto const diff = centerPositionShot - centerPositionPlayer;
    auto const lSquared = jt::MathHelper::lengthSquared(diff);
    if (lSquared
        <= (playerHalfSize.x() + shotHalfSize.x()) * (playerHalfSize.y() + shotHalfSize.y())) {
        s._alive = false;
        playerTakeDamage(player, s);
    }
}

void performPlayerEnemyCollision(PlayerState& player, EnemyState& enemy)
{
    auto const playerHalfSize = jt::Vector2 { Game::GameProperties::playerSizeInPixel() / 2.0f,
        Game::GameProperties::playerSizeInPixel() / 2.0f };
    auto const enemyHalfSize = Game::GameProperties::enemyHalfSize();
    auto const centerPositionPlayer = player.position + playerHalfSize;
    auto const centerPositionEnemy = enemy.position + enemyHalfSize;

    auto const diff = centerPositionPlayer - centerPositionEnemy;
    auto const lSquared = jt::MathHelper::lengthSquared(diff);
    if (lSquared
        <= (playerHalfSize.x() + enemyHalfSize.x()) * (playerHalfSize.y() + enemyHalfSize.y())) {
        enemy._alive = false;
        playerTakeDamage(player, enemy);
    }
}

int main()
{
    NetworkServer server;

    PlayerMap playerStates;
    std::map<int, std::size_t> playerPredictionId;

    std::map<int, jt::CircularBuffer<std::size_t, 20>> receivedMessageIdsMap;

    std::vector<ShotState> shots;
    std::vector<EnemyState> enemies;

    EnemySpawner spawner { enemies };

    float elapsed = 0.0f;
    std::cout << "starting server...\n";
    while (true) {
        auto now = std::chrono::steady_clock::now();

        auto next = now
            + std::chrono::milliseconds { static_cast<long long>(
                Network::NetworkProperties::serverTickTime() * 1000) };

        auto allPlayerIds = server.getAllPlayerIds();

        removeInactivePlayers(playerStates, allPlayerIds);

        std::vector<int> playersToDisconnect {};
        for (auto currentPlayerId : allPlayerIds) {

            createNewPlayerIfNotKnownToServer(playerStates, currentPlayerId);

            auto dataForPlayer = server.getData(currentPlayerId);
            if (dataForPlayer.size() > 1) {
                std::sort(dataForPlayer.begin(), dataForPlayer.end(),
                    [](auto const& payloadA, auto const& payloadB) {
                        return payloadA.messageId < payloadB.messageId;
                    });
            }
            for (auto& payload : dataForPlayer) {
                std::size_t const messageId = payload.messageId;
                if (checkForDuplicatedMessages(receivedMessageIdsMap, currentPlayerId, messageId)) {
                    continue;
                }
                if (payload.disconnect == true) {
                    playersToDisconnect.push_back(currentPlayerId);
                    break;
                }

                updatePlayerState(playerStates[currentPlayerId], payload.dt, payload.input);
                playerPredictionId[currentPlayerId] = payload.currentPredictionId;

                if (payload.input[jt::KeyCode::Space]) {
                    if (playerStates[currentPlayerId]._shootTimer <= 0) {
                        auto const shotPosition = playerStates[currentPlayerId].position
                            + jt::Vector2 { Game::GameProperties::playerSizeInPixel() / 2.0f, 0.0f }
                            - jt::Vector2 { 4.0f, 0.0f };
                        auto const shotDirection = getShotJitterDirection(
                            Game::GameProperties::shotBaseJitterAmount(), jt::Vector2 { 0, -1 });
                        shots.emplace_back(ShotState { shotPosition, shotDirection });
                        playerStates[currentPlayerId]._shootTimer
                            = Game::GameProperties::playerShootCooldown();
                    }
                }
            }
        }

        for (auto playerToDisconnectId : playersToDisconnect) {
            server.closeConnectionTo(playerToDisconnectId);
        }

        spawner.setActivePlayerCount(static_cast<int>(playerStates.size()));
        spawner.setCurrentlyAliveEnemies(static_cast<int>(enemies.size()));
        // TODO increase difficulty
        spawner.setDifficulty(1.0f);
        spawner.update(elapsed);
        for (auto& e : enemies) {
            updateEnemyState(e, shots, elapsed);
            for (auto& p : playerStates) {
                performPlayerEnemyCollision(p.second, e);
            }
        }

        for (auto& s : shots) {
            updateShotState(s, elapsed);

            if (s.fromPlayer) {

                for (auto& e : enemies) {
                    performShotEnemyCollision(s, e);
                }
            } else {
                for (auto& p : playerStates) {
                    performShotPlayerCollision(p.second, s);
                }
            }
        }

        jt::SystemHelper::erase_if(shots,
            [](auto& s) { return s._age >= Game::GameProperties::shotLifeTime() || !s._alive; });

        jt::SystemHelper::erase_if(enemies, [](auto& e) { return !e._alive; });

        for (auto& kvp : playerStates) {
            PayloadServer2Client payload;
            payload.playerID = kvp.first;
            payload.playerStates = playerStates;
            payload.prediction_id = playerPredictionId[kvp.first];
            payload.shots = shots;
            payload.enemies = enemies;
            server.sendToClient(kvp.first, payload);
        }

        std::this_thread::sleep_until(next);
        auto after = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::microseconds>(after - now).count()
            / 1000.0f / 1000.0f;
    }
}
