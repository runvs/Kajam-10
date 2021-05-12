#include "StateGame.hpp"
#include "Box2D/Box2D.h"
#include "ClientProperties.hpp"
#include "Color.hpp"
#include "GameInterface.hpp"
#include "Hud.hpp"
#include "Payloads.hpp"
#include "PlayerState.hpp"
#include "Shape.hpp"
#include "Sprite.hpp"
#include "StateMenu.hpp"
#include "SystemHelper.hpp"
#include "TweenAlpha.hpp"

void StateGame::doInternalCreate()
{
    m_world = std::make_shared<b2World>(b2Vec2 { 0.0f, 0.0f });

    float const w = static_cast<float>(GP::GetWindowSize().x());
    float const h = static_cast<float>(GP::GetWindowSize().y());

    using jt::Shape;
    using jt::TweenAlpha;

    m_background = std::make_shared<Shape>();
    m_background->makeRect({ w, h });
    m_background->setColor(GP::PaletteBackground());
    m_background->setIgnoreCamMovement(true);
    m_background->update(0.0f);

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
    m_vignette->loadSprite("#v#" + std::to_string(static_cast<int>(GP::GetScreenSize().x())) + "#"
        + std::to_string(static_cast<int>(GP::GetScreenSize().y())));
    m_vignette->setIgnoreCamMovement(true);
    m_vignette->setColor({ 255, 255, 255, 100 });

    m_localPlayer = std::make_shared<Player>(true);
    add(m_localPlayer);

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    // StateGame will call drawObjects itself.
    setAutoDraw(false);

    m_client = std::make_shared<NetworkClient>(sf::IpAddress { "217.80.132.122" });
    m_client->send(PayloadClient2Server { 0, {} });
}

void StateGame::updateActivePlayerPositionFromServer(
    int playerID, std::shared_ptr<Player> player, PlayerMap playerPositions)
{
    player_state = playerPositions.at(playerID);
    player->m_shape->setPosition(player_state.position);
}

void StateGame::spawnNewPlayer(int newPlayerId)
{
    m_remotePlayers[newPlayerId] = std::make_shared<Player>(false);
    m_remotePlayers[newPlayerId]->setGameInstance(getGame());
    m_remotePlayers[newPlayerId]->create();
}

void StateGame::UpdateAllPlayerPositionsFromServer(PayloadServer2Client payload)
{
    auto activePlayerId = payload.playerID;

    auto playerPositions = payload.playerStates;

    for (auto kvp : playerPositions) {
        if (kvp.first == activePlayerId) {
            m_localPlayer->m_shape->setPosition(kvp.second.position);
        } else {
            if (m_remotePlayers.count(kvp.first) == 0) {
                spawnNewPlayer(kvp.first);
            }
            m_remotePlayers[kvp.first]->m_shape->setPosition(kvp.second.position);
        }
    }
    updateActivePlayerPositionFromServer(activePlayerId, m_localPlayer, playerPositions);
}

void StateGame::removeLocalOnlyPlayers(PayloadServer2Client payload)
{
    if (m_remotePlayers.size() == payload.playerStates.size()) {
        return;
    }
    jt::SystemHelper::erase_if(m_remotePlayers,
        [&payload](auto const kvp) { return payload.playerStates.count(kvp.first) == 0; });
}

void StateGame::doInternalUpdate(float const elapsed)
{
    if (m_running) {
        // TODO Reneable box2d update only if required
        // m_world->Step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());
        // update game logic here
        auto inputState = m_localPlayer->getInput();

        // TODO use correct predictionId
        // TODO use correct player id
        const PayloadClient2Server payload { 0, inputState, elapsed, 0, false };
        m_client->send(payload);

        if (m_client->isNewDataAvailable()) {
            auto payload = m_client->getData();
            UpdateAllPlayerPositionsFromServer(payload);
            removeLocalOnlyPlayers(payload);
        } else {
            updatePlayerState(player_state, elapsed, inputState);

            m_localPlayer->m_shape->setPosition(player_state.position);
        }

        const std::size_t buffer_index
            = current_prediction_id & Network::NetworkProperties::c_buffer_mask();
        predicted_move[buffer_index].dt = elapsed;
        predicted_move[buffer_index].input = inputState;
        predicted_move_result[buffer_index] = player_state;
        ++current_prediction_id;
    }

    m_background->update(elapsed);
    m_vignette->update(elapsed);
    m_overlay->update(elapsed);
    for (auto p : m_remotePlayers) {
        p.second->update(elapsed);
    }
}

void StateGame::doInternalDraw() const
{
    m_background->draw(getGame()->getRenderTarget());
    drawObjects();
    m_localPlayer->draw();
    for (auto p : m_remotePlayers) {
        p.second->draw();
    }
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
