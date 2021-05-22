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

bool overlaps(jt::Vector2 const& aPos, jt::Vector2 const& aHalfSize, jt::Vector2 const& bPos,
    jt::Vector2 const& bHalfSize)
{
    auto const aCenterPosition = aPos + aHalfSize;
    auto const bCenterPosition = bPos + bHalfSize;
    auto const diff = aCenterPosition - bCenterPosition;
    auto const lSquared = jt::MathHelper::lengthSquared(diff);
    return (lSquared < (aHalfSize.x() + bHalfSize.x()) * (aHalfSize.y() + bHalfSize.y()));
}

void GameServer::performShotEnemyCollision(ShotState& shot, EnemyState& enemy)
{
    if (overlaps(shot.position, Game::GameProperties::shotHalfSize(), enemy.position,
            Game::GameProperties::enemyHalfSize())) {
        shot._alive = false;
        enemyTakeDamage(enemy, shot);
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

    if (overlaps(shot.position, Game::GameProperties::shotHalfSize(), player.position,
            Game::GameProperties::playerHalfSize())) {
        shot._alive = false;
        playerTakeDamage(player, shot);

        if (player.health <= 0) {
            m_score -= Game::GameProperties::scorePlayerDeathMalus();
        }
    }
}

void GameServer::performPlayerEnemyCollision(EnemyState& enemy, PlayerState& player)
{
    if (overlaps(player.position, Game::GameProperties::playerHalfSize(), enemy.position,
            Game::GameProperties::enemyHalfSize())) {
        enemy._alive = false;
        playerTakeDamage(player, enemy);
        if (player.health <= 0) {
            m_score -= Game::GameProperties::scorePlayerDeathMalus();
        }
    }
}

void GameServer::performAllPlayerEnemyCollision()
{
    for (auto& enemy : m_enemies) {
        for (auto& p : m_playerStates) {
            auto& player = p.second;
            if (player.health <= 0) {
                continue;
            }

            performPlayerEnemyCollision(enemy, player);
        }
    }
}

void GameServer::sortIncomingPayloadsForPlayer(
    std::vector<PayloadClient2Server> dataForPlayer) const
{
    std::sort(
        dataForPlayer.begin(), dataForPlayer.end(), [](auto const& payloadA, auto const& payloadB) {
            return payloadA.messageId < payloadB.messageId;
        });
}

ShotState GameServer::createPlayerShot(jt::Vector2 const& playerPosition) const
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

void GameServer::removeDeadEnemies()
{
    jt::SystemHelper::erase_if(m_enemies, [](auto& e) { return !e._alive; });
}

void GameServer::removeDeadShots()
{
    jt::SystemHelper::erase_if(m_shots,
        [](auto& s) { return s._age >= Game::GameProperties::shotLifeTime() || !s._alive; });
}

void GameServer::removeDeadPowerups()
{
    jt::SystemHelper::erase_if(m_powerups, [](auto& p) { return !p._alive; });
}

void GameServer::handlePowerupEffect(
    std::vector<PowerupState>::value_type& powerup, PlayerState& player)
{
    powerup._alive = false;
    // TODO other powerup types
    if (powerup.type == static_cast<int>(PowerupType::POWERUP_HEALTH)) {
        player.health += Game::GameProperties::powerupHealthAmount();
        player.health
            = jt::MathHelper::clamp(player.health, 0, Game::GameProperties::playerMaxHealth());
    }
}

bool GameServer::performPlayerPowerupCollision(PowerupState& powerup, PlayerState& player)
{
    if (player.health <= 0) {
        return true;
    }
    if (overlaps(powerup.position, Game::GameProperties::powerupHalfSize(), player.position,
            Game::GameProperties::playerHalfSize())) {
        handlePowerupEffect(powerup, player);
    }
    return false;
}

void GameServer::updateAllPowerups()
{
    for (auto& powerup : m_powerups) {
        if (powerup._alive) {
            continue;
        }
        for (auto& p : m_playerStates) {
            if (performPlayerPowerupCollision(powerup, p.second))
                continue;
        }
    }
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
    auto const now = std::chrono::steady_clock::now();

    auto const next = now
        + std::chrono::milliseconds { static_cast<long long>(
            Network::NetworkProperties::serverTickTime() * 1000) };

    removeInactivePlayers();

    handleAllPayloadsForAllPlayers();

    removeDisconnectedPlayers();

    handleEnemySpawning();

    updateAllEnemies();

    performAllPlayerEnemyCollision();

    updateAllShots();
    handleAllShotCollisions();

    removeDeadShots();
    removeDeadEnemies();

    updateAllPowerups();

    m_score = jt::MathHelper::clamp(m_score, 0, Game::GameProperties::scoreMax());

    sendPayloadToPlayers();

    std::this_thread::sleep_until(next);
    auto after = std::chrono::steady_clock::now();
    m_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(after - now).count() / 1000.0f
        / 1000.0f;
}
