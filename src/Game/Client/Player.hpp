#ifndef PLAYER_HPP_GUARD
#define PLAYER_HPP_GUARD
#include "GameObject.hpp"
#include "KeyCodes.hpp"
#include "Shape.hpp"
class Player : public jt::GameObject {
public:
    std::map<jt::KeyCode, bool> getInput();
    std::shared_ptr<jt::Shape> m_shape;

private:
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void doCreate() override;
    void doKill() override;
    void doDestroy() override;

    std::map<jt::KeyCode, bool> m_currentInput;
};

#endif
