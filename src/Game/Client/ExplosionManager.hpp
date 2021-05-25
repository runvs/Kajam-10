#ifndef EXPLOSION_GUARD_HPP
#define EXPLOSION_GUARD_HPP

#include "ExplosionState.hpp"
#include "GameObject.hpp"
#include "Shape.hpp"
#include "Vector.hpp"
#include <memory>
#include <vector>

class ExplosionManager : public jt::GameObject {
public:
    ExplosionManager();
    void add(ExplosionState const& explosionState);

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    void removeDeadExplosions();

    std::vector<ExplosionState> m_explosions;

    std::shared_ptr<jt::Shape> m_shape;
};

#endif // EXPLOSION_GUARD_HPP
