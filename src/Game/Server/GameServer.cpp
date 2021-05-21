#include "GameServer.hpp"
#include "SystemHelper.hpp"
#include "common.hpp"

#include <iostream>

void GameServer::removeInactivePlayers()
{
    auto const playerIds = m_networkServer.getAllPlayerIds();
    jt::SystemHelper::erase_if(m_playerStates, [&playerIds](auto const& kvp) {
        return std::find(playerIds.cbegin(), playerIds.cend(), kvp.first) == playerIds.cend();
    });
}

jt::Vector2 GameServer::getRandomNewPlayerPosition() const
{
    auto const minXPos
        = Game::GameProperties::hudBarMargin() * 2.0f + Game::GameProperties::hudBarWidth();
    float const xPos = jt::Random::getFloat(minXPos, Game::GameProperties::displayScreenSize().x());
    float const yPos = Game::GameProperties::displayScreenSize().y() * 0.8f;
    return jt::Vector2 { xPos, yPos };
}

void GameServer::createNewPlayerIfNotKnownToServer(int currentPlayerId)
{
    bool isPlayerKnownToServer = (m_playerStates.count(currentPlayerId) != 0);
    if (isPlayerKnownToServer) {
        return;
    }

    m_playerStates[currentPlayerId].position = getRandomNewPlayerPosition();
}

bool GameServer::checkForDuplicatedMessages(int currentPlayerId, std::size_t const messageId)
{
    bool hasDuplicateMessage = m_receivedMessageIdsMap[currentPlayerId].contains(messageId);
    if (hasDuplicateMessage) {
        std::cout << "skip duplicated message for player " << currentPlayerId << std::endl;
    } else {
        m_receivedMessageIdsMap[currentPlayerId].push(messageId);
    }

    return hasDuplicateMessage;
}

void GameServer::performShotEnemyCollision(ShotState& s, EnemyState& enemy)
{
    auto const enemyHalfSize = Game::GameProperties::enemyHalfSize();
    auto const shotHalfSize = Game::GameProperties::shotHalfSize();
    auto const centerPositionEnemy = enemy.position + enemyHalfSize;
    auto const centerPositionShot = s.position + shotHalfSize;
    auto const diff = centerPositionShot - centerPositionEnemy;
    auto const lSquared = jt::MathHelper::lengthSquared(diff);

    bool isOverlapping = lSquared
        < (enemyHalfSize.x() + shotHalfSize.x()) * (enemyHalfSize.y() + shotHalfSize.y());
    if (isOverlapping) {
        s._alive = false;
        enemyTakeDamage(enemy, s);
        if (!enemy._alive) {
            m_score += Game::GameProperties::scoreEnemyKillBonus();
        }
    }
}

void GameServer::performShotPlayerCollision(PlayerState& player, ShotState& shot)
{
    if (player.health <= 0) {
        return;
    }
    auto const playerHalfSize = jt::Vector2 { Game::GameProperties::playerSizeInPixel() / 2.0f,
        Game::GameProperties::playerSizeInPixel() / 2.0f };
    auto const shotHalfSize = Game::GameProperties::shotHalfSize();
    auto const centerPositionPlayer = player.position + playerHalfSize;
    auto const centerPositionShot = shot.position + shotHalfSize;
    auto const diff = centerPositionShot - centerPositionPlayer;
    auto const lSquared = jt::MathHelper::lengthSquared(diff);

    bool isOverlapping = lSquared
        <= (playerHalfSize.x() + shotHalfSize.x()) * (playerHalfSize.y() + shotHalfSize.y());
    if (isOverlapping) {
        shot._alive = false;
        playerTakeDamage(player, shot);

        if (player.health <= 0) {
            m_score -= Game::GameProperties::scorePlayerDeathMalus();
        }
    }
}

void GameServer::performPlayerEnemyCollision()
{
    for (auto& enemy : m_enemies) {
        for (auto& p : m_playerStates) {
            auto& player = p.second;
            if (player.health <= 0) {
                continue;
            }
            auto const playerHalfSize
                = jt::Vector2 { Game::GameProperties::playerSizeInPixel() / 2.0f,
                      Game::GameProperties::playerSizeInPixel() / 2.0f };
            auto const enemyHalfSize = Game::GameProperties::enemyHalfSize();
            auto const centerPositionPlayer = player.position + playerHalfSize;
            auto const centerPositionEnemy = enemy.position + enemyHalfSize;

            auto const diff = centerPositionPlayer - centerPositionEnemy;
            auto const lSquared = jt::MathHelper::lengthSquared(diff);

            bool isOverlapping = lSquared <= (playerHalfSize.x() + enemyHalfSize.x())
                    * (playerHalfSize.y() + enemyHalfSize.y());
            if (isOverlapping) {
                enemy._alive = false;
                playerTakeDamage(player, enemy);
                if (player.health <= 0) {
                    m_score -= Game::GameProperties::scorePlayerDeathMalus();
                }
            }
        }
    }
}

void GameServer::sortIncomingPayloadsForPlayer(std::vector<PayloadClient2Server> dataForPlayer)
{
    std::sort(
        dataForPlayer.begin(), dataForPlayer.end(), [](auto const& payloadA, auto const& payloadB) {
            return payloadA.messageId < payloadB.messageId;
        });
}

ShotState GameServer::createPlayerShot(jt::Vector2 const& playerPosition)
{
    auto const shotOffset
        = jt::Vector2 { static_cast<float>(Game::GameProperties::playerSizeInPixel()) / 2.0f - 4.0f,
              0.0f };
    auto const shotPosition = playerPosition + shotOffset;
    auto const shotDirection = getShotJitterDirection(
        jt::Vector2 { 0, -1 }, Game::GameProperties::shotBaseJitterAmount());
    auto shotState = ShotState { shotPosition, shotDirection };
    return shotState;
}

void GameServer::handlePlayerShooting(int currentPlayerId, PayloadClient2Server payload)
{
    bool const hasPlayerPressedShootKey = payload.input.at(jt::KeyCode::Space);
    if (hasPlayerPressedShootKey) {
        bool const hasPlayerShootTimerExpired = m_playerStates[currentPlayerId]._shootTimer <= 0;
        if (hasPlayerShootTimerExpired) {
            m_shots.emplace_back(createPlayerShot(m_playerStates[currentPlayerId].position));
            m_playerStates[currentPlayerId]._shootTimer
                = Game::GameProperties::playerShootCooldown();
        }
    }
}

bool GameServer::handleSinglePayloadForSinglePlayer(
    int currentPlayerId, PayloadClient2Server const& payload)
{
    std::size_t const messageId = payload.messageId;
    if (checkForDuplicatedMessages(currentPlayerId, messageId)) {
        return false;
    }
    if (payload.disconnect == true) {
        m_playersToDisconnect.push_back(currentPlayerId);
        return true;
    }

    updatePlayerState(m_playerStates[currentPlayerId], payload.dt, payload.input);

    playerPredictionId[currentPlayerId] = payload.currentPredictionId;

    handlePlayerShooting(currentPlayerId, payload);
    return false;
}

void GameServer::handleAllPayloadForSinglePlayer(int currentPlayerId)
{
    createNewPlayerIfNotKnownToServer(currentPlayerId);

    auto dataForPlayer = m_networkServer.getData(currentPlayerId);

    sortIncomingPayloadsForPlayer(dataForPlayer);

    for (auto const& payload : dataForPlayer) {
        if (handleSinglePayloadForSinglePlayer(currentPlayerId, payload)) {
            break;
        }
    }
}

void GameServer::handleAllPayloadsForAllPlayers()
{
    for (auto currentPlayerId : m_networkServer.getAllPlayerIds()) {

        handleAllPayloadForSinglePlayer(currentPlayerId);
    }
}

void GameServer::removeDisconnectedPlayers()
{
    for (auto playerToDisconnectId : m_playersToDisconnect) {
        m_networkServer.closeConnectionTo(playerToDisconnectId);
    }
    m_playersToDisconnect.clear();
}

void GameServer::handleEnemySpawning()
{
    m_enemySpawner.setActivePlayerCount(static_cast<int>(m_playerStates.size()));
    // TODO increase difficulty
    m_enemySpawner.setDifficulty(1.0f);
    m_enemySpawner.update(m_elapsed);
}

void GameServer::updateAllEnemies()
{
    for (auto& e : m_enemies) {
        updateEnemyState(e, m_shots, m_elapsed);
    }
}

void GameServer::updateAllShots()
{
    for (auto& s : m_shots) {
        updateShotState(s, m_elapsed);
    }
}

void GameServer::handleSingleShotCollision(ShotState& s)
{
    if (s.fromPlayer) {
        for (auto& e : m_enemies) {
            performShotEnemyCollision(s, e);
        }
    } else {
        for (auto& p : m_playerStates) {
            performShotPlayerCollision(p.second, s);
        }
    }
}

void GameServer::handleAllShotCollisions()
{
    for (auto& s : m_shots) {
        handleSingleShotCollision(s);
    }
}

void GameServer::removeDeadShots()
{
    jt::SystemHelper::erase_if(m_shots,
        [](auto& s) { return s._age >= Game::GameProperties::shotLifeTime() || !s._alive; });
}

void GameServer::removeDeadEnemies()
{
    jt::SystemHelper::erase_if(m_enemies, [](auto& e) { return !e._alive; });
}

void GameServer::sendSinglePayloadToPlayer(std::pair<int, PlayerState> const& kvp)
{
    PayloadServer2Client payload;
    payload.playerID = kvp.first;
    payload.playerStates = m_playerStates;
    payload.predictionId = playerPredictionId[kvp.first];
    payload.shots = m_shots;
    payload.enemies = m_enemies;
    payload.score = m_score;
    m_networkServer.sendToClient(kvp.first, payload);
}

void GameServer::sendPayloadToPlayers()
{
    for (auto& kvp : m_playerStates) {
        sendSinglePayloadToPlayer(kvp);
    }
}

void GameServer::update()
{
    auto now = std::chrono::steady_clock::now();

    auto next = now
        + std::chrono::milliseconds { static_cast<long long>(
            Network::NetworkProperties::serverTickTime() * 1000) };

    removeInactivePlayers();

    handleAllPayloadsForAllPlayers();

    removeDisconnectedPlayers();

    handleEnemySpawning();

    updateAllEnemies();

    performPlayerEnemyCollision();

    updateAllShots();
    handleAllShotCollisions();

    removeDeadShots();
    removeDeadEnemies();

    m_score = jt::MathHelper::clamp(m_score, 0, Game::GameProperties::scoreMax());

    sendPayloadToPlayers();

    std::this_thread::sleep_until(next);
    auto after = std::chrono::steady_clock::now();
    m_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(after - now).count() / 1000.0f
        / 1000.0f;
}
