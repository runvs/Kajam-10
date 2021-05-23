#ifndef GAME_HUD_HPP_INCLUDEGUARD
#define GAME_HUD_HPP_INCLUDEGUARD

#include "Bar.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"
#include "Text.hpp"

class Hud : public jt::GameObject {
public:
    Hud();

    void setScore(int i);
    void setHealth(int health);

    bool m_connectedToServer { false };

private:
    int m_score { -1 };

    jt::Text::Sptr m_scoreText;
    std::shared_ptr<jt::Bar> m_scoreBar;

    std::shared_ptr<jt::Bar> m_healthBar;

    std::shared_ptr<jt::Shape> mutable m_barBackgroundShape;

    std::shared_ptr<jt::Text> m_textNotConnectedToServer;

    void doUpdate(float const elapsed) override;

    void doDraw() const override;

    void doCreate() override;

    void createTextNotConnectedToServer();
};

#endif
