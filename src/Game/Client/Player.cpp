#include "Player.hpp"
#include "DrawableHelpers.hpp"
#include "GameInterface.hpp"

void Player::doCreate() { m_shape = jt::dh::createRectShape({ 32, 32 }); }

void Player::doUpdate(float const elapsed)
{

    for (auto k : jt::getAllKeys()) {
        m_currentInput[k] = getGame()->input()->keyboard()->pressed(k);
    }
    m_shape->update(elapsed);
}
void Player::doDraw() const { m_shape->draw(getGame()->getRenderTarget()); }

void Player::doKill() { }
void Player::doDestroy() { }

std::map<jt::KeyCode, bool> Player::getInput() { return m_currentInput; }
