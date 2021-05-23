#include "Player.hpp"
#include "DrawableHelpers.hpp"
#include "GameInterface.hpp"
#include "common.hpp"

std::vector<jt::KeyCode> getNeededKeys()
{
    std::vector<jt::KeyCode> keys;
    keys.push_back(jt::KeyCode::W);
    keys.push_back(jt::KeyCode::S);
    keys.push_back(jt::KeyCode::A);
    keys.push_back(jt::KeyCode::D);
    keys.push_back(jt::KeyCode::Space);
    keys.push_back(jt::KeyCode::Num1);
    keys.push_back(jt::KeyCode::Num2);
    keys.push_back(jt::KeyCode::Num3);
    keys.push_back(jt::KeyCode::Num4);
    keys.push_back(jt::KeyCode::Num5);
    return keys;
}

// TODO load sprite instead of create shape
void Player::doCreate()
{
    m_sprite = std::make_shared<jt::Animation>();
    if (m_isLocalPlayer) {
        m_sprite->add("assets/turtle1.jpg", "idle", jt::Vector2u { 16, 16 }, { 0, 1 }, 0.15f);
    } else {
        m_sprite->add("assets/turtle2.jpg", "idle", jt::Vector2u { 16, 16 }, { 0, 1 }, 0.15f);
    }
    m_sprite->play("idle");
}

void Player::updateInput()
{
    static auto neededKeys = getNeededKeys();
    for (auto k : neededKeys) {
        m_currentInput[k] = getGame()->input()->keyboard()->pressed(k);
    }
}

void Player::setColorBasedOnLocalOrRemote() { m_sprite->setColor(jt::colors::White); }

void Player::flashPlayerIfDead(float const elapsed)
{
    if (m_health <= 0) {
        m_flickerTimer -= elapsed;
        if (m_flickerTimer <= 0) {
            m_flickerTimer = 0.1f;
            if (m_sprite->getColor().a() == 255) {
                m_sprite->setColor(jt::colors::Transparent);
            } else {
                setColorBasedOnLocalOrRemote();
            }
        }
    } else {

        setColorBasedOnLocalOrRemote();
    }
}

void Player::doUpdate(float const elapsed)
{
    if (m_isLocalPlayer) {
        updateInput();
    }
    flashPlayerIfDead(elapsed);
    m_sprite->update(elapsed);
}
void Player::doDraw() const { m_sprite->draw(getGame()->getRenderTarget()); }

void Player::doKill() { }
void Player::doDestroy() { }

Player::Player(bool isLocal) { m_isLocalPlayer = isLocal; }

std::map<jt::KeyCode, bool> Player::getInput() { return m_currentInput; }

void Player::setHealth(int health) { m_health = health; }
