#include "ExplosionManager.hpp"
#include "ClientProperties.hpp"
#include "Color.hpp"
#include "DrawableHelpers.hpp"
#include "GameInterface.hpp"
#include "GameProperties.hpp"
#include "Lerp.hpp"
#include "MathHelper.hpp"
#include "Random.hpp"
#include "SystemHelper.hpp"
#include <iostream>
#include <memory>

ExplosionManager::ExplosionManager() = default;

void ExplosionManager::doCreate()
{
    m_shapeFire = jt::dh::createRectShape(
        { static_cast<float>(Game::GameProperties::playerSizeInPixel()),
            static_cast<float>(Game::GameProperties::playerSizeInPixel()) },
        jt::Color { 180, 32, 32, 255 });

    createShockwave();
    createSmokeSprites();
}

void ExplosionManager::createSmokeSprites()
{
    for (int i = 0; i != 10; ++i) {
        auto shape = jt::dh::createRectShape(
            jt::Vector2 { static_cast<float>(Game::GameProperties::playerSizeInPixel()),
                static_cast<float>(Game::GameProperties::playerSizeInPixel()) },
            jt::Color { 255, 255, 255, static_cast<std::uint8_t>(255) });
        m_shapesSmoke.emplace_back(shape);
    }
}

void ExplosionManager::createShockwave()
{
    m_spriteShockwave = std::make_shared<jt::Animation>();
    auto animationSpeed = 0.01f;
    m_spriteShockwave->add(
        "assets/explosionShockwave.png", "fade0", jt::Vector2u { 16, 16 }, { 0 }, animationSpeed);
    m_spriteShockwave->add(
        "assets/explosionShockwave.png", "fade1", jt::Vector2u { 16, 16 }, { 1 }, animationSpeed);
    m_spriteShockwave->add(
        "assets/explosionShockwave.png", "fade2", jt::Vector2u { 16, 16 }, { 2 }, animationSpeed);
    m_spriteShockwave->add(
        "assets/explosionShockwave.png", "fade3", jt::Vector2u { 16, 16 }, { 3 }, animationSpeed);
    m_spriteShockwave->add(
        "assets/explosionShockwave.png", "fade4", jt::Vector2u { 16, 16 }, { 4 }, animationSpeed);
    m_spriteShockwave->add(
        "assets/explosionShockwave.png", "fade5", jt::Vector2u { 16, 16 }, { 5 }, animationSpeed);
    m_spriteShockwave->add(
        "assets/explosionShockwave.png", "none", jt::Vector2u { 16, 16 }, { 6 }, animationSpeed);
    m_spriteShockwave->play("none");
}

void ExplosionManager::add(ExplosionState const& explosionState)
{
    explosionState._rotation = jt::Random::getFloat(0.0f, 360.0f);
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
    m_spriteShockwave->setOrigin(jt::Vector2 { 8.0f, 8.0f });

    auto shockwaveScale = GP::explosionShockwaveBaseSize()
        + GP::explosionShockwaveMaxSize() * explosionState._progress;
    m_spriteShockwave->setScale(jt::Vector2 { shockwaveScale, shockwaveScale });

    if (explosionState._progress <= 0.1f) {
        m_spriteShockwave->play("fade0");
    } else if (explosionState._progress <= 0.2f) {
        m_spriteShockwave->play("fade1");
    } else if (explosionState._progress <= 0.3f) {
        m_spriteShockwave->play("fade2");
    } else if (explosionState._progress <= 0.35f) {
        m_spriteShockwave->play("fade3");
    } else if (explosionState._progress <= 0.4f) {
        m_spriteShockwave->play("fade4");
    } else if (explosionState._progress <= 0.43f) {
        m_spriteShockwave->play("fade5");
    } else {
        m_spriteShockwave->play("none");
    }
}

namespace {
const std::map<int, std::pair<float, jt::Vector2>> smokeBasePositions {
    { 0, { 15.0f, jt::Vector2 { 0.66f, 0.75f } } },
    { 1, { 15.0f, jt::Vector2 { -0.3f, 0.95f } } },
    { 2, { 15.0f, jt::Vector2 { -0.96f, -0.3f } } },
    { 3, { 15.0f, jt::Vector2 { -0.46f, -0.89f } } },
    { 4, { 15.0f, jt::Vector2 { -0.87f, -0.49f } } },
    { 5, { 15.0f, jt::Vector2 { -0.87f, 0.5f } } },
    { 6, { 15.0f, jt::Vector2 { 0.21f, -0.98f } } },
    { 7, { 15.0f, jt::Vector2 { 0.42f, 0.91f } } },
    { 8, { 15.8f, jt::Vector2 { -1.0f, 0.07f } } },
    { 9, { 15.3f, jt::Vector2 { 1.0f, 0.07f } } },
};
} // namespace

void ExplosionManager::prepareSmokeShapesByProgress(ExplosionState const& explosionState) const
{
    for (int i = 0; i != 10; ++i) {
        float distance
            = jt::Lerp::cosine(0.0f, smokeBasePositions.at(i).first, explosionState._progress);
        auto offset = jt::MathHelper::rotateBy(
            distance * smokeBasePositions.at(i).second, explosionState._rotation);
        m_shapesSmoke.at(i)->setPosition(explosionState.position + offset);
    }
}

void ExplosionManager::doDraw() const
{
    for (auto& e : m_explosions) {
        prepareShapesByProgress(e);
        prepareSmokeShapesByProgress(e);

        m_spriteShockwave->update(0.1f);
        m_spriteShockwave->draw(getGame()->getRenderTarget());

        m_shapeFire->update(0.1f);
        m_shapeFire->draw(getGame()->getRenderTarget());

        for (auto& s : m_shapesSmoke) {
            s->update(0.1f);
            s->draw(getGame()->getRenderTarget());
        }
    }
}