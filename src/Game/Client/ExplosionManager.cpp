#include "ExplosionManager.hpp"
#include "DrawableHelpers.hpp"
#include "GameInterface.hpp"
#include "GameProperties.hpp"
#include "SystemHelper.hpp"

ExplosionManager::ExplosionManager() = default;

void ExplosionManager::doCreate()
{
    std::cout << "Creatign Explosion" << std::endl;
    m_shape = jt::dh::createRectShape(
        { static_cast<float>(Game::GameProperties::playerSizeInPixel()),
            static_cast<float>(Game::GameProperties::playerSizeInPixel()) },
        jt::Color { 180, 32, 32, 255 });
}

void ExplosionManager::add(ExplosionState const& explosionState)
{
    m_explosions.emplace_back(explosionState);
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
}

void ExplosionManager::doDraw() const
{
    for (auto& e : m_explosions) {
        m_shape->setPosition(e.position);
        m_shape->update(0.1f);
        m_shape->draw(getGame()->getRenderTarget());
    }
}