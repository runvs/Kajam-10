#ifndef PLAYER_HPP_GUARD
#define PLAYER_HPP_GUARD
#include "GameObject.hpp"
#include "KeyCodes.hpp"
#include "Shape.hpp"
class Player : public jt::GameObject {
public:
    Player(bool isLocal);
    std::map<jt::KeyCode, bool> getInput();
    void setHealth(int health);
    std::shared_ptr<jt::Shape> m_shape;

private:
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void doCreate() override;
    void updateInput();
    void setColorBasedOnLocalOrRemote();
    void flashPlayerIfDead(float elapsed);
    void doKill() override;
    void doDestroy() override;

    std::map<jt::KeyCode, bool> m_currentInput;
    bool m_isLocalPlayer;
    int m_health;
    float m_flickerTimer { 0.0f };
};

#endif
