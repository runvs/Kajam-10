﻿#include "GameServer.hpp"
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

void GameServer::performPlayerEnemyCollision(PlayerState& player, EnemyState& enemy)
{
    if (player.health <= 0) {
        return;
    }
    auto const playerHalfSize = jt::Vector2 { Game::GameProperties::playerSizeInPixel() / 2.0f,
        Game::GameProperties::playerSizeInPixel() / 2.0f };
    auto const enemyHalfSize = Game::GameProperties::enemyHalfSize();
    auto const centerPositionPlayer = player.position + playerHalfSize;
    auto const centerPositionEnemy = enemy.position + enemyHalfSize;

    auto const diff = centerPositionPlayer - centerPositionEnemy;
    auto const lSquared = jt::MathHelper::lengthSquared(diff);

    bool isOverlapping = lSquared
        <= (playerHalfSize.x() + enemyHalfSize.x()) * (playerHalfSize.y() + enemyHalfSize.y());
    if (isOverlapping) {
        enemy._alive = false;
        playerTakeDamage(player, enemy);

        if (player.health <= 0) {
            m_score -= Game::GameProperties::scorePlayerDeathMalus();
        }
    }
}

void GameServer::update()
{
    auto now = std::chrono::steady_clock::now();

    auto next = now
        + std::chrono::milliseconds { static_cast<long long>(
            Network::NetworkProperties::serverTickTime() * 1000) };
    removeInactivePlayers();

    std::vector<int> playersToDisconnect {};
    for (auto currentPlayerId : m_networkServer.getAllPlayerIds()) {

        createNewPlayerIfNotKnownToServer(currentPlayerId);

        auto dataForPlayer = m_networkServer.getData(currentPlayerId);
        if (dataForPlayer.size() > 1) {
            std::sort(dataForPlayer.begin(), dataForPlayer.end(),
                [](auto const& payloadA, auto const& payloadB) {
                    return payloadA.messageId < payloadB.messageId;
                });
        }
        for (auto& payload : dataForPlayer) {
            std::size_t const messageId = payload.messageId;
            if (checkForDuplicatedMessages(currentPlayerId, messageId)) {
                continue;
            }
            if (payload.disconnect == true) {
                playersToDisconnect.push_back(currentPlayerId);
                break;
            }

            updatePlayerState(m_playerStates[currentPlayerId], payload.dt, payload.input);
            playerPredictionId[currentPlayerId] = payload.currentPredictionId;

            if (payload.input[jt::KeyCode::Space]) {
                if (m_playerStates[currentPlayerId]._shootTimer <= 0) {
                    auto const shotPosition = m_playerStates[currentPlayerId].position
                        + jt::Vector2 { Game::GameProperties::playerSizeInPixel() / 2.0f, 0.0f }
                        - jt::Vector2 { 4.0f, 0.0f };
                    auto const shotDirection = getShotJitterDirection(
                        Game::GameProperties::shotBaseJitterAmount(), jt::Vector2 { 0, -1 });
                    shots.emplace_back(ShotState { shotPosition, shotDirection });
                    m_playerStates[currentPlayerId]._shootTimer
                        = Game::GameProperties::playerShootCooldown();
                }
            }
        }
    }

    for (auto playerToDisconnectId : playersToDisconnect) {
        m_networkServer.closeConnectionTo(playerToDisconnectId);
    }

    spawner.setActivePlayerCount(static_cast<int>(m_playerStates.size()));
    spawner.setCurrentlyAliveEnemies(static_cast<int>(enemies.size()));
    // TODO increase difficulty
    spawner.setDifficulty(1.0f);
    spawner.update(elapsed);
    for (auto& e : enemies) {
        updateEnemyState(e, shots, elapsed);
        for (auto& p : m_playerStates) {
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
            for (auto& p : m_playerStates) {
                performShotPlayerCollision(p.second, s);
            }
        }
    }
    m_score = jt::MathHelper::clamp(m_score, 0, Game::GameProperties::scoreMax());

    jt::SystemHelper::erase_if(
        shots, [](auto& s) { return s._age >= Game::GameProperties::shotLifeTime() || !s._alive; });

    jt::SystemHelper::erase_if(enemies, [](auto& e) { return !e._alive; });

    for (auto& kvp : m_playerStates) {
        PayloadServer2Client payload;
        payload.playerID = kvp.first;
        payload.playerStates = m_playerStates;
        payload.predictionId = playerPredictionId[kvp.first];
        payload.shots = shots;
        payload.enemies = enemies;
        payload.score = m_score;
        m_networkServer.sendToClient(kvp.first, payload);
    }

    std::this_thread::sleep_until(next);
    auto after = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(after - now).count() / 1000.0f
        / 1000.0f;
}
