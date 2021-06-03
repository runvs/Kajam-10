#include "Parallax.hpp"
#include "DrawableHelpers.hpp"
#include "GameInterface.hpp"
#include "GameProperties.hpp"

void Parallax::doCreate()
{
    float const max = 10.0f;
    for (int i = 0; i != 100; ++i) {
        float const v = jt::Random::getFloat(1.0f, max);
        float const vinv = (max - v) / max;
        float alpha = (0.25f + 0.75f * vinv) * 200;
        auto shape = jt::dh::createRectShape(
            jt::Vector2 { 2, v }, jt::Color { 255, 255, 255, static_cast<std::uint8_t>(alpha) });
        shape->setPosition(jt::Random::getRandomPointin(
            jt::Rect { 0, -m_margin, Game::GameProperties::displayScreenSize().x(),
                Game::GameProperties::displayScreenSize().y() + m_margin }));
        m_shapes.emplace(std::make_pair(v * 0.15f, shape));
    }
}

void Parallax::doUpdate(float const elapsed)
{
    for (auto& kvp : m_shapes) {
        auto const pos = kvp.second->getPosition();
        jt::Vector2 const delta
            = { 0.0f, elapsed * Game::GameProperties::scrollSpeed() * kvp.first };
        auto newpos = pos + delta;
        if (newpos.y() >= Game::GameProperties::displayScreenSize().y() + m_margin) {
            newpos.y() = -m_margin;
        }
        kvp.second->setPosition(newpos);

        kvp.second->update(elapsed);
    }
}

void Parallax::doDraw() const
{
    for (auto const& kvp : m_shapes) {
        kvp.second->draw(getGame()->getRenderTarget());
    }
}
