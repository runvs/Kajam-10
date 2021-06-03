#ifndef PARALLAX_HPP_GUARD
#define PARALLAX_HPP_GUARD

#include "GameObject.hpp"
#include "Shape.hpp"

class Parallax : public jt::GameObject {
public:
    Parallax() = default;

private:
    std::multimap<float, std::shared_ptr<jt::Shape>> m_shapes;
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;

    float m_margin { 64.0f };
};

#endif
