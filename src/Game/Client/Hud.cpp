#include "Hud.hpp"
#include "ClientProperties.hpp"
#include "Color.hpp"
#include "DrawableHelpers.hpp"
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

    auto const margin = Game::GameProperties::hudBarMargin();
    m_healthBar = std::make_shared<jt::Bar>(Game::GameProperties::hudBarWidth(),
        Game::GameProperties::displayScreenSize().y() - 2.0f * margin, false);
    m_healthBar->setPosition({ margin, margin });
    m_healthBar->setFrontColor(jt::Color { 102, 165, 35, 255 });
    m_healthBar->setMaxValue(static_cast<float>(Game::GameProperties::playerMaxHealth()));

    m_scoreBar = std::make_shared<jt::Bar>(Game::GameProperties::hudBarWidth(),
        Game::GameProperties::displayScreenSize().y() - 2.0f * margin, false);
    m_scoreBar->setPosition({ Game::GameProperties::displayScreenSize().x() - margin
            - Game::GameProperties::hudBarWidth(),
        margin });
    m_scoreBar->setMaxValue(static_cast<float>(Game::GameProperties::scoreMax()));

    m_barBackgroundShape
        = jt::dh::createRectShape({ Game::GameProperties::hudBarWidth() * 2.0f,
                                      Game::GameProperties::displayScreenSize().y() },
            jt::Color { 32, 32, 32, 255 });

    createTextNotConnectedToServer();
}

void Hud::createTextNotConnectedToServer()
{
    float half_width = Game::GameProperties::displayScreenSize().x() / 2;
    m_textNotConnectedToServer = jt::dh::createText(
        getGame()->getRenderTarget(), "Not connected to Server!", 16U, GP::PaletteFontDanger());
    m_textNotConnectedToServer->setPosition({ half_width, 20 });
    m_textNotConnectedToServer->setShadow(GP::PaletteFontShadow(), jt::Vector2 { 3, 3 });
}

void Hud::setScore(int i)
{
    m_score = i;
    m_scoreBar->setCurrentValue(static_cast<float>(m_score));
}

void Hud::setHealth(int health)
{
    m_healthBar->setCurrentValue(static_cast<float>(health));

    if (health < Game::GameProperties::playerMaxHealth() * 0.33f) {
        m_healthBar->setFrontColor(jt::Color { 168, 0, 0, 255 });
    } else if (health < Game::GameProperties::playerMaxHealth() * 0.67f) {
        m_healthBar->setFrontColor(jt::Color { 205, 192, 43, 255 });
    } else {
        m_healthBar->setFrontColor(jt::Color { 102, 165, 35, 255 });
    }
}

void Hud::doUpdate(float const elapsed)
{
    if (m_score >= 0) {
        m_scoreText->setText("P1 Score: " + std::to_string(m_score));
    }

    m_scoreText->update(elapsed);
    m_scoreBar->update(elapsed);
    m_healthBar->update(elapsed);
    m_textNotConnectedToServer->update(elapsed);
}

void Hud::doDraw() const
{
    if (m_score >= 0) {
        m_scoreText->draw(getGame()->getRenderTarget());
    }

    // Draw bar background on the left
    m_barBackgroundShape->setPosition(jt::Vector2 { 0, 0 });
    m_barBackgroundShape->update(0.1f);
    m_barBackgroundShape->draw(getGame()->getRenderTarget());

    // Draw bar background on the right
    m_barBackgroundShape->setPosition(jt::Vector2 { Game::GameProperties::displayScreenSize().x()
            - (Game::GameProperties::hudBarWidth() * 2.0f),
        0 });
    m_barBackgroundShape->update(0.1f);
    m_barBackgroundShape->draw(getGame()->getRenderTarget());

    m_healthBar->draw(getGame()->getRenderTarget());
    m_scoreBar->draw(getGame()->getRenderTarget());

    if (!m_connectedToServer) {
        m_textNotConnectedToServer->draw(getGame()->getRenderTarget());
    }
}
