#include "Hud.hpp"
#include "Color.hpp"
#include "GameInterface.hpp"
#include "GameProperties.hpp"

Hud::Hud() = default;

void Hud::doCreate()
{
    m_scoreText = std::make_shared<jt::Text>();
    m_scoreText->loadFont("assets/font.ttf", 16, getGame()->getRenderTarget());
    m_scoreText->setColor(jt::Color { 248, 249, 254 });
    m_scoreText->update(0.0f);
    m_scoreText->SetTextAlign(jt::Text::TextAlign::LEFT);
    m_scoreText->setPosition({ 20, 325 });

    auto const margin = 4;
    m_healthBar = std::make_shared<jt::Bar>(
        8.0f, Game::GameProperties::displayScreenSize().y() - 2.0f * margin, false);
    m_healthBar->setPosition({ margin, margin });
    m_healthBar->setMaxValue(static_cast<float>(Game::GameProperties::playerMaxHealth()));
}

void Hud::addScore(int i)
{
    m_score += i;
    if (m_score < 0) {
        m_score = 0;
    }
}

void Hud::setHealth(int health) { m_healthBar->setCurrentValue(static_cast<float>(health)); }

void Hud::doUpdate(float const elapsed)
{
    if (m_score >= 0) {
        m_scoreText->setText("P1 Score: " + std::to_string(m_score));
    }

    m_scoreText->update(elapsed);
    m_healthBar->update(elapsed);
}

void Hud::doDraw() const
{
    if (m_score >= 0) {
        m_scoreText->draw(getGame()->getRenderTarget());
    }
    m_healthBar->draw(getGame()->getRenderTarget());
}
