#include "Player.hpp"
#include "DrawableHelpers.hpp"
#include "GameInterface.hpp"

std::vector<jt::KeyCode> getNeededKeys()
{
    std::vector<jt::KeyCode> keys;
    keys.push_back(jt::KeyCode::W);
    keys.push_back(jt::KeyCode::S);
    keys.push_back(jt::KeyCode::A);
    keys.push_back(jt::KeyCode::D);
    keys.push_back(jt::KeyCode::Space);
    return keys;
}

void Player::doCreate() { m_shape = jt::dh::createRectShape({ 32, 32 }); }

void Player::updateInput()
{
    static auto neededKeys = getNeededKeys();
    for (auto k : neededKeys) {
        m_currentInput[k] = getGame()->input()->keyboard()->pressed(k);
    }
}

void Player::doUpdate(float const elapsed)
{
    if (m_isActivePlayer) {
        updateInput();
        m_shape->setColor(jt::colors::Green);
    }
    m_shape->update(elapsed);
}
void Player::doDraw() const { m_shape->draw(getGame()->getRenderTarget()); }

void Player::doKill() { }
void Player::doDestroy() { }

Player::Player(bool isActive) { m_isActivePlayer = isActive; }

std::map<jt::KeyCode, bool> Player::getInput() { return m_currentInput; }
