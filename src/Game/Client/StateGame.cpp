#include "StateGame.hpp"
#include "Box2D/Box2D.h"
#include "ClientProperties.hpp"
#include "Color.hpp"
#include "DrawableHelpers.hpp"
#include "GameInterface.hpp"
#include "GameProperties.hpp"
#include "Hud.hpp"
#include "MathHelper.hpp"
#include "Parallax.hpp"
#include "Payloads.hpp"
#include "PlayerState.hpp"
#include "Shape.hpp"
#include "Sound.hpp"
#include "Soundgroup.hpp"
#include "Sprite.hpp"
#include "StateMenu.hpp"
#include "SystemHelper.hpp"
#include "TweenAlpha.hpp"
#include <cmath>
#include <iostream>

void StateGame::doInternalCreate()
{
    m_world = std::make_shared<b2World>(b2Vec2 { 0.0f, 0.0f });

    float const w = static_cast<float>(Game::GameProperties::displayWindowSize().x());
    float const h = static_cast<float>(Game::GameProperties::displayWindowSize().y());

    using jt::Shape;
    using jt::TweenAlpha;

    m_background = std::make_shared<Shape>();
    m_background->makeRect({ w, h });
    m_background->setColor(jt::Color { 25, 25, 25 });
    m_background->setIgnoreCamMovement(true);
    m_background->update(0.0f);

    m_parallax = std::make_shared<Parallax>();
    add(m_parallax);

    m_overlay = std::make_shared<Shape>();
    m_overlay->makeRect(jt::Vector2 { w, h });
    m_overlay->setColor(jt::Color { 0, 0, 0 });
    m_overlay->setIgnoreCamMovement(true);
    m_overlay->update(0);
    auto tw = TweenAlpha<Shape>::create(m_overlay, 0.5f, std::uint8_t { 255 }, std::uint8_t { 0 });
    tw->setSkipFrames();
    tw->addCompleteCallback([this]() { m_running = true; });
    add(tw);

    m_vignette = std::make_shared<jt::Sprite>();
    m_vignette->loadSprite("#v#"
        + std::to_string(static_cast<int>(Game::GameProperties::displayScreenSize().x())) + "#"
        + std::to_string(static_cast<int>(Game::GameProperties::displayScreenSize().y())));
    m_vignette->setIgnoreCamMovement(true);
    m_vignette->setColor({ 255, 255, 255, 100 });

    m_shotSprite = std::make_shared<jt::Animation>();
    m_shotSprite->add("assets/shots.png", "idle", { 16, 16 }, { 0, 1 }, 0.1f);
    m_shotSprite->play("idle");
    m_enemySprite = std::make_shared<jt::Sprite>();
    m_enemySprite->loadSprite("assets/enemy.png");

    m_powerupAnimation = std::make_shared<jt::Animation>();
    m_powerupAnimation->add("assets/powerup_repair.png", "repair", { 16, 16 }, { 0 }, 0.1f);
    m_powerupAnimation->add("assets/powerup_points.png", "points", { 16, 16 }, { 0 }, 0.1f);
    m_powerupAnimation->add("assets/powerup_shot.png", "shot", { 16, 16 }, { 0 }, 0.1f);

    m_localPlayer = std::make_shared<Player>(true);
    add(m_localPlayer);

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    m_explosionManager = std::make_shared<ExplosionManager>();
    add(m_explosionManager);

    // StateGame will call drawObjects itself.
    setAutoDraw(false);

    m_client
        = std::make_shared<NetworkClient>(sf::IpAddress { Network::NetworkProperties::serverIp() });
    m_client->send(PayloadClient2Server { 0, {} });

    m_shotSounds = std::make_shared<jt::SoundGroup>(std::vector<std::string> {
        "assets/sfx/shoot1.ogg", "assets/sfx/shoot2.ogg", "assets/sfx/shoot3.ogg" });
    m_shotSounds->setVolume(50);

    m_powerupSound = std::make_shared<jt::Sound>();
    m_powerupSound->load("assets/sfx/powerup1.ogg");
}

void StateGame::updateActivePlayerPositionFromServer(
    std::shared_ptr<Player> player, PlayerMap playerPositions)
{
    m_currentPlayerState = playerPositions.at(m_localPlayerId);
    player->m_sprite->setPosition(m_currentPlayerState.position);
}

void StateGame::spawnNewPlayer(int newPlayerId)
{
    m_remotePlayers[newPlayerId] = std::make_shared<Player>(false);
    m_remotePlayers[newPlayerId]->setGameInstance(getGame());
    m_remotePlayers[newPlayerId]->create();
}

void StateGame::updateRemotePlayers(PayloadServer2Client payload)
{
    auto playerStates = payload.playerStates;

    for (auto kvp : playerStates) {
        if (kvp.first != m_localPlayerId) {
            if (m_remotePlayers.count(kvp.first) == 0) {
                spawnNewPlayer(kvp.first);
            }
            m_remotePlayers[kvp.first]->m_sprite->setPosition(kvp.second.position);
            m_remotePlayers[kvp.first]->setHealth(kvp.second.health);
        }
    }
}

void StateGame::removeLocalOnlyPlayers(PayloadServer2Client payload)
{
    jt::SystemHelper::erase_if(m_remotePlayers,
        [&payload](auto const kvp) { return payload.playerStates.count(kvp.first) == 0; });
}

void StateGame::checkLocalPlayerId(int const payloadPlayerId)
{
    if (m_localPlayerId == -1) {
        m_localPlayerId = payloadPlayerId;
        std::cout << "Local player was assigned id: " << payloadPlayerId << std::endl;
    } else {
        if (m_localPlayerId != payloadPlayerId) {
            std::cout << "Error: player id in payload does not match local player id\n";
        }
    }
}

void StateGame::doInternalUpdate(float const elapsed)
{
    if (m_running) {
        // TODO Reneable box2d update only if required
        // m_world->Step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());
        // update game logic here
        auto inputState = m_localPlayer->getInput();

        m_secondsSincelastDataReceived += elapsed;
        if (m_secondsSincelastDataReceived >= GP::ClientToServerTimeout()) {
            m_hud->m_connectedToServer = false;
        }

        m_parallax->update(elapsed);

        const PayloadClient2Server payload { m_localPlayerId, inputState, elapsed,
            m_currentPredictionId, false };
        m_client->send(payload);

        m_predictedMoves[m_currentPredictionId].elapsed = elapsed;
        m_predictedMoves[m_currentPredictionId].input = inputState;
        m_predictedMoveResults[m_currentPredictionId] = m_currentPlayerState;
        ++m_currentPredictionId;

        updatePlayerState(m_currentPlayerState, elapsed, inputState);

        if (m_client->isNewDataAvailable()) {
            m_secondsSincelastDataReceived = 0.0f;
            m_hud->m_connectedToServer = true;

            auto payloads = m_client->getData();
            auto lastPayload = *payloads.rbegin();

            for (auto const& payload : payloads) {
                removeLocalOnlyPlayers(payload);
                checkLocalPlayerId(payload.playerID);
                if (payload.shotFired) {
                    m_shotSounds->play();
                }
                if (payload.playerStates.at(payload.playerID).pickedUpPowerup) {
                    m_localPlayer->m_sprite->flash(0.25f);
                    m_powerupSound->play();
                }
                if (payload.playerStates.at(payload.playerID).takenDamage) {
                    m_overlay->flash(0.25f, jt::colors::Red);
                    m_localPlayer->m_sprite->flash(0.55f, jt::colors::Red);
                }
                for (auto const& e : payload.explosions) {
                    m_explosionManager->add(e);
                }
            }

            m_shots = lastPayload.shots;
            m_enemies = lastPayload.enemies;
            m_powerups = lastPayload.powerups;
            m_hud->setHealth(lastPayload.playerStates.at(m_localPlayerId).health);
            m_hud->setScore(lastPayload.score);
            m_localPlayer->setHealth(lastPayload.playerStates.at(m_localPlayerId).health);
            m_currentPlayerState.health = lastPayload.playerStates.at(m_localPlayerId).health;

            auto const diff = m_predictedMoveResults[lastPayload.predictionId].position
                - lastPayload.playerStates[m_localPlayerId].position;

            if (jt::MathHelper::lengthSquared(diff)
                > Game::GameProperties::playerMaxAllowedPredictionError()) {
                std::cout << "prediction error for prediction id " << lastPayload.predictionId
                          << "with local current prediction id: " << m_currentPredictionId
                          << std::endl;
                m_currentPlayerState = lastPayload.playerStates[m_localPlayerId];

                for (std::size_t replayingPredictionId = lastPayload.predictionId + 1;
                     replayingPredictionId < m_currentPredictionId; ++replayingPredictionId) {

                    updatePlayerState(m_currentPlayerState,
                        m_predictedMoves[replayingPredictionId].elapsed,
                        m_predictedMoves[replayingPredictionId].input);

                    m_predictedMoveResults[replayingPredictionId] = m_currentPlayerState;
                }
            }
            updateRemotePlayers(lastPayload);
        }
    }
    m_localPlayer->m_sprite->setPosition(m_currentPlayerState.position);
    m_background->update(elapsed);
    m_vignette->update(elapsed);
    m_overlay->update(elapsed);

    for (auto p : m_remotePlayers) {
        p.second->update(elapsed);
    }
}

void StateGame::drawPlayerShots() const
{
    for (auto& s : m_shots) {
        auto angleInDegrees = -jt::MathHelper::rad2deg(atan2(s.direction.y(), s.direction.x()));
        m_shotSprite->setOrigin(Game::GameProperties::shotHalfSizeRotation());
        if (s.fromPlayer) {
            m_shotSprite->setColor(jt::Color { 255, 255, 255 });
            m_shotSprite->setRotation(angleInDegrees);
            m_shotSprite->setPosition(s.position);
            m_shotSprite->update(0.1f);
            m_shotSprite->draw(getGame()->getRenderTarget());
        }
    }
}

void StateGame::drawEnemyShots() const
{
    for (auto& s : m_shots) {
        auto angleInDegrees = -jt::MathHelper::rad2deg(atan2(s.direction.y(), s.direction.x()));
        m_shotSprite->setOrigin(Game::GameProperties::shotHalfSizeRotation());
        if (!s.fromPlayer) {
            m_shotSprite->setColor(jt::Color { 255, 150, 150 });
            m_shotSprite->setRotation(angleInDegrees);
            m_shotSprite->setPosition(s.position);
            m_shotSprite->update(0.1f);
            m_shotSprite->draw(getGame()->getRenderTarget());
        }
    }
}

void StateGame::doInternalDraw() const
{
    m_background->draw(getGame()->getRenderTarget());

    drawObjects();
    m_parallax->draw();
    drawPlayerShots();
    drawEnemyShots();

    for (auto& p : m_powerups) {
        float const sinOffset = p.position.x() + p.position.y();
        float const offset = sin(getAge() * 1.1f + sinOffset) * 8.0f;
        m_powerupAnimation->setPosition(p.position + jt::Vector2 { 0.0f, offset });
        if (p.type == static_cast<int>(PowerupType::POWERUP_HEALTH)) {
            m_powerupAnimation->play("repair", 0, true);
        } else if (p.type == static_cast<int>(PowerupType::POWERUP_POINTS)) {
            m_powerupAnimation->play("points", 0, true);
        } else
            m_powerupAnimation->play("shot", 0, true);
        m_powerupAnimation->update(0.1f);
        m_powerupAnimation->draw(getGame()->getRenderTarget());
    }

    for (auto& e : m_enemies) {
        m_enemySprite->setPosition(e.position);
        m_enemySprite->update(0.1f);
        m_enemySprite->draw(getGame()->getRenderTarget());
    }

    for (auto p : m_remotePlayers) {
        p.second->draw();
    }
    m_localPlayer->draw();
    m_vignette->draw(getGame()->getRenderTarget());
    m_hud->draw();
    m_overlay->draw(getGame()->getRenderTarget());
}

void StateGame::endGame()
{
    if (m_hasEnded) {
        // trigger this function only once
        return;
    }
    m_hasEnded = true;
    m_running = false;

    auto tw = jt::TweenAlpha<jt::Shape>::create(
        m_overlay, 0.5f, std::uint8_t { 0 }, std::uint8_t { 255 });
    tw->setSkipFrames();
    tw->addCompleteCallback([this]() { getGame()->switchState(std::make_shared<StateMenu>()); });
    add(tw);
}
