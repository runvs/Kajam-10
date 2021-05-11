#include "StateGame.hpp"
#include "../Common/Payloads.hpp"
#include "Box2D/Box2D.h"
#include "Color.hpp"
#include "GameInterface.hpp"
#include "GameProperties.hpp"
#include "Hud.hpp"
#include "Shape.hpp"
#include "Sprite.hpp"
#include "StateMenu.hpp"
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

    m_player = std::make_shared<Player>(true);
    add(m_player);

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    // StateGame will call drawObjects itself.
    setAutoDraw(false);

    m_client = std::make_shared<NetworkClient>(sf::IpAddress { "127.0.0.1" });
    m_client->send(PayloadClient2Server { 0, {} });
}

void StateGame::updateActivePlayerPositionFromServer(
    int playerID, std::shared_ptr<Player> player, std::map<int, jt::Vector2> playerPositions)
{
    auto pos = playerPositions.at(playerID);
    player->m_shape->setPosition(pos);
}

void StateGame::spawnNewPlayer(int newPlayerId)
{
    m_players[newPlayerId] = std::make_shared<Player>(false);
}

void StateGame::UpdateAllPlayerPositionsFromServer(PayloadServer2Client payload)
{
    auto activePlayerId = payload.playerID;

    auto playerPositions = payload.playerPositions;

    for (auto kvp : playerPositions) {
        if (kvp.first == activePlayerId) {
            m_player->m_shape->setPosition(kvp.second);
        } else {
            if (m_players.count(kvp.first) == 0) {
                spawnNewPlayer(kvp.first);
            }
            m_players[kvp.first]->m_shape->setPosition(kvp.second);
        }
    }
    updateActivePlayerPositionFromServer(activePlayerId, m_player, playerPositions);
}

void StateGame::InterpolateActivePlayer(float const elapsed)
{
    auto pos = m_player->m_shape->getPosition();
    if (getGame()->input()->keyboard()->pressed(jt::KeyCode::D))
        pos.x() += elapsed * 100;
    else if (getGame()->input()->keyboard()->pressed(jt::KeyCode::A))
        pos.x() -= elapsed * 100;

    if (getGame()->input()->keyboard()->pressed(jt::KeyCode::W))
        pos.y() -= elapsed * 100;
    else if (getGame()->input()->keyboard()->pressed(jt::KeyCode::S))
        pos.y() += elapsed * 100;

    m_player->m_shape->setPosition(pos);
}

void StateGame::doInternalUpdate(float const elapsed)
{
    if (m_running) {
        m_world->Step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());
        // update game logic here
        if (m_client->isNewDataAvailable()) {
            auto payload = m_client->getData();
            UpdateAllPlayerPositionsFromServer(payload);

        } else {
            InterpolateActivePlayer(elapsed);
        }

        const PayloadClient2Server payload { 0, m_player->getInput() };
        m_client->send(payload);
    }

    m_background->update(elapsed);
    m_vignette->update(elapsed);
    m_overlay->update(elapsed);
}

void StateGame::doInternalDraw() const
{
    m_background->draw(getGame()->getRenderTarget());
    drawObjects();
    m_player->draw();
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
