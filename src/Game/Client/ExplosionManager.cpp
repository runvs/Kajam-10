#include "ExplosionManager.hpp"
#include "DrawableHelpers.hpp"
#include "GameInterface.hpp"
#include "GameProperties.hpp"
#include "Lerp.hpp"
#include "SystemHelper.hpp"
#include <iostream>

ExplosionManager::ExplosionManager() = default;

void ExplosionManager::doCreate()
{
    m_shapeFire = jt::dh::createRectShape(
        { static_cast<float>(Game::GameProperties::playerSizeInPixel()),
            static_cast<float>(Game::GameProperties::playerSizeInPixel()) },
        jt::Color { 180, 32, 32, 255 });
    m_spriteShockwave = std::make_shared<jt::Animation>();
    m_spriteShockwave->add(
        "assets/explosionShockwave.png", "idle", jt::Vector2u { 16, 16 }, { 0, 1 }, 0.1f);
    m_spriteShockwave->play("idle");
}

void ExplosionManager::add(ExplosionState const& explosionState)
{
    m_explosions.emplace_back(explosionState);
    getGame()->getCamera()->shake(0.3f, 1.5f);
}

void ExplosionManager::removeDeadExplosions()
{
    jt::SystemHelper::erase_if(m_explosions, [](auto& e) { return !e._alive; });
}

void ExplosionManager::doUpdate(float const elapsed)
{
    for (auto& e : m_explosions) {
        updateExplosionState(e, elapsed);
    }

    removeDeadExplosions();
}

void ExplosionManager::prepareShapesByProgress(ExplosionState const& explosionState) const
{
    auto const opacity
        = static_cast<uint8_t>(255 - jt::Lerp::cubic(0.0f, 255.0f, explosionState._progress));

    m_shapeFire->setPosition(explosionState.position);
    m_shapeFire->setColor(jt::Color { 180, 32, 32, opacity });

    m_spriteShockwave->setPosition(explosionState.position + Game::GameProperties::enemyHalfSize());
    // m_spriteShockwave->setOrigin(Game::GameProperties::explosionSize());
    m_spriteShockwave->setOrigin(jt::Vector2 { 8.0f, 8.0f });
    m_spriteShockwave->setScale(jt::Vector2 {
        1.0f + 3.0f * explosionState._progress, 1.0f + 3.0f * explosionState._progress });
}

void ExplosionManager::doDraw() const
{
    for (auto& e : m_explosions) {
        prepareShapesByProgress(e);

        m_spriteShockwave->update(0.1f);
        m_spriteShockwave->draw(getGame()->getRenderTarget());

        m_shapeFire->update(0.1f);
        m_shapeFire->draw(getGame()->getRenderTarget());
    }
}