#include "GameServer.hpp"
#include "PlayerShotSpawner.hpp"
#include "SystemHelper.hpp"
#include "common.hpp"
#include <iostream>

namespace {

bool overlaps(jt::Vector2 const& aPos, jt::Vector2 const& aHalfSize, jt::Vector2 const& bPos,
    jt::Vector2 const& bHalfSize)
{
    auto const aCenterPosition = aPos + aHalfSize;
    auto const bCenterPosition = bPos + bHalfSize;
    auto const diff = aCenterPosition - bCenterPosition;
    auto const lSquared = jt::MathHelper::lengthSquared(diff);
    return (lSquared < (aHalfSize.x() + bHalfSize.x()) * (aHalfSize.y() + bHalfSize.y()));
}

jt::Vector2 getRandomNewPlayerPosition()
{
    auto const minXPos
        = Game::GameProperties::hudBarMargin() * 2.0f + Game::GameProperties::hudBarWidth();
    float const xPos = jt::Random::getFloat(minXPos, Game::GameProperties::displayScreenSize().x());
    float const yPos = Game::GameProperties::displayScreenSize().y() * 0.8f;
    return jt::Vector2 { xPos, yPos };
}

void increasePlayerShotLevel(PlayerState& player)
{
    if (player._shotPattern == Shots::ShotPattern::level1()) {
        player._shotPattern = Shots::ShotPattern::level2();
    } else if (player._shotPattern == Shots::ShotPattern::level2()) {
        player._shotPattern = Shots::ShotPattern::level3();
    } else if (player._shotPattern == Shots::ShotPattern::level3()) {
        player._shotPattern = Shots::ShotPattern::level4();
    } else if (player._shotPattern == Shots::ShotPattern::level4()) {
        player._shotPattern = Shots::ShotPattern::level5();
    } else if (player._shotPattern == Shots::ShotPattern::level5()) {
        player._shotPattern = Shots::ShotPattern::level6();
    }
}

} // namespace

void GameServer::resetServerState()
{
    if (m_playerStates.empty()) {
        // std::cout << "no players connected, reset server\n";
        m_score = 0;
        m_level = 0;
        m_enemies.clear();
        m_powerups.clear();
        m_enemyKillCount = 0;
        m_shots.clear();
    }
}

void GameServer::removeInactivePlayers()
{
    auto const playerIds = m_networkServer.getAllPlayerIds();
    jt::SystemHelper::erase_if(m_playerStates, [&playerIds](auto const& kvp) {
        return std::find(playerIds.cbegin(), playerIds.cend(), kvp.first) == playerIds.cend();
    });

    resetServerState();
}

void GameServer::createNewPlayerIfNotKnownToServer(int currentPlayerId)
{
    const bool isPlayerKnownToServer = (m_playerStates.count(currentPlayerId) != 0);
    if (isPlayerKnownToServer) {
        return;
    }

    m_playerStates[currentPlayerId].position = getRandomNewPlayerPosition();
    m_playerStates[currentPlayerId]._shotPattern = Shots::ShotPattern::SINGLE();
}

bool GameServer::checkForDuplicatedMessages(int currentPlayerId, std::size_t const messageId)
{
    const bool hasDuplicateMessage = m_receivedMessageIdsMap[currentPlayerId].contains(messageId);
    if (hasDuplicateMessage) {
        std::cout << "skip duplicated message for player " << currentPlayerId << std::endl;
    } else {
        m_receivedMessageIdsMap[currentPlayerId].push(messageId);
    }

    return hasDuplicateMessage;
}

void GameServer::increaseLevel()
{
    m_level++;
    m_score = 0;
}

void GameServer::performShotEnemyCollision(ShotState& shot, EnemyState& enemy)
{
    if (overlaps(shot.position - Game::GameProperties::shotHalfSizeCollision(),
            Game::GameProperties::shotHalfSizeCollision(), enemy.position,
            Game::GameProperties::enemyHalfSize())
        && enemy.position.y() >= Game::GameProperties::enemyHalfSize().y()) {
        shot._alive = false;
        enemyTakeDamage(enemy, shot);
        if (!enemy._alive) {
            m_score += Game::GameProperties::scoreEnemyKillBonus();
            if (m_score >= Game::GameProperties::scoreMax()) {
                increaseLevel();
            }
            m_enemyKillCount++;
            spawnNewPowerups(enemy.position);
            spawnNewExplosion(enemy.position);
        }
    }
}

void GameServer::performShotPlayerCollision(PlayerState& player, ShotState& shot)
{
    if (player.health <= 0) {
        return;
    }

    if (overlaps(shot.position, Game::GameProperties::shotHalfSizeCollision(), player.position,
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

void GameServer::handlePlayerShooting(int currentPlayerId, PayloadClient2Server payload)
{
    bool const hasPlayerPressedShootKey = payload.input.at(jt::KeyCode::Space);
    if (hasPlayerPressedShootKey) {
        bool const isPlayerShootTimerExpired = m_playerStates[currentPlayerId]._shootTimer <= 0;

        if (isPlayerShootTimerExpired) {
            m_shotFired = true;
            auto shots = PlayerShotSpawner::createShotFromPattern(
                m_playerStates[currentPlayerId]._shotPattern,
                m_playerStates[currentPlayerId].position);
            for (auto const& s : shots) {
                m_shots.emplace_back(s);
            }
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
    float const v = jt::MathHelper::clamp(static_cast<float>(m_score)
                            / static_cast<float>(Game::GameProperties::scoreMax())
                            * Game::GameProperties::enemyHealthIncrease(),
                        1.0f, Game::GameProperties::enemyHealthIncrease())
        + m_level;
    m_enemySpawner.setDifficulty(v);
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

void GameServer::clearExplosions() { m_explosions.clear(); }

int GameServer::getPowerupType()
{
    auto const allPlayersHaveFullHp
        = std::all_of(m_playerStates.cbegin(), m_playerStates.cend(), [](auto const& kvp) {
              return kvp.second.health == Game::GameProperties::playerMaxHealth();
          });
    auto const allPlayersHaveFullShot = std::all_of(m_playerStates.cbegin(), m_playerStates.cend(),
        [](auto const& kvp) { return kvp.second._shotPattern == Shots::ShotPattern::level6(); });

    if (allPlayersHaveFullHp && allPlayersHaveFullShot) {
        return static_cast<int>(PowerupType::POWERUP_POINTS);
    }
    if (allPlayersHaveFullHp) {
        return jt::Random::getInt(1, static_cast<int>(PowerupType::POWERUP_MAXNUMBER) - 1);
    }
    if (allPlayersHaveFullShot) {
        return jt::Random::getInt(0, static_cast<int>(PowerupType::POWERUP_MAXNUMBER) - 2);
    }
    return jt::Random::getInt(0, static_cast<int>(PowerupType::POWERUP_MAXNUMBER) - 1);
}

void GameServer::spawnNewPowerups(jt::Vector2 const& enemyPosition)
{
    if (m_enemyKillCount == Game::GameProperties::enemyKillsNeededForPowerup()) {
        m_enemyKillCount = 0;
        PowerupState p;
        p.position = enemyPosition;
        p.type = getPowerupType();
        m_powerups.push_back(p);
    }
}

void GameServer::updateAllPowerups()
{
    for (auto& powerup : m_powerups) {
        if (!powerup._alive) {
            continue;
        }
        for (auto& p : m_playerStates) {
            performPlayerPowerupCollision(powerup, p.second);
        }
    }
}

void GameServer::handlePowerupEffect(PowerupState& powerup, PlayerState& player)
{
    powerup._alive = false;
    player.pickedUpPowerup = true;
    // TODO other powerup types
    if (powerup.type == static_cast<int>(PowerupType::POWERUP_HEALTH)) {
        player.health += Game::GameProperties::powerupHealthAmount();
        player.health
            = jt::MathHelper::clamp(player.health, 0, Game::GameProperties::playerMaxHealth());
    } else if (powerup.type == static_cast<int>(PowerupType::POWERUP_SHOT)) {
        increasePlayerShotLevel(player);
    } else if (powerup.type == static_cast<int>(PowerupType::POWERUP_POINTS)) {
        m_score += Game::GameProperties::scorePowerupBoostAmount();
    }
}

void GameServer::performPlayerPowerupCollision(PowerupState& powerup, PlayerState& player)
{
    if (player.health <= 0) {
        return;
    }
    if (overlaps(powerup.position, Game::GameProperties::powerupHalfSize(), player.position,
            Game::GameProperties::playerHalfSize())) {
        handlePowerupEffect(powerup, player);
    }
}

void GameServer::spawnNewExplosion(jt::Vector2 const& enemyPosition)
{
    auto explosion = ExplosionState { enemyPosition };
    m_explosions.emplace_back(explosion);
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
    payload.level = m_level;
    payload.powerups = m_powerups;
    payload.explosions = m_explosions;
    payload.shotFired = m_shotFired;
    m_networkServer.sendToClient(kvp.first, payload);
}

void GameServer::sendPayloadToPlayers()
{
    for (auto& kvp : m_playerStates) {
        sendSinglePayloadToPlayer(kvp);
    }
}

void GameServer::resetPlayerStatesForThisFrame()
{
    for (auto& p : m_playerStates) {
        auto& player = p.second;
        player.takenDamage = false;
        player.pickedUpPowerup = false;
    }
}

void GameServer::update()
{
    m_shotFired = false;
    auto const now = std::chrono::steady_clock::now();

    auto const next = now
        + std::chrono::milliseconds { static_cast<long long>(
            Network::NetworkProperties::serverTickTime() * 1000) };

    removeInactivePlayers();
    clearExplosions();

    handleAllPayloadsForAllPlayers();

    removeDisconnectedPlayers();

    handleEnemySpawning();

    updateAllEnemies();

    resetPlayerStatesForThisFrame();

    performAllPlayerEnemyCollision();

    updateAllShots();
    handleAllShotCollisions();

    updateAllPowerups();

    removeDeadShots();
    removeDeadEnemies();
    removeDeadPowerups();

    m_score = jt::MathHelper::clamp(m_score, 0, Game::GameProperties::scoreMax());

    sendPayloadToPlayers();

    std::this_thread::sleep_until(next);
    const auto after = std::chrono::steady_clock::now();
    m_elapsed = static_cast<float>(
                    std::chrono::duration_cast<std::chrono::microseconds>(after - now).count())
        / 1000.0f / 1000.0f;
}
