#ifndef EXPLOSION_GUARD_HPP
#define EXPLOSION_GUARD_HPP

#include "Animation.hpp"
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

    void createShockwave();
    void createSmokeSprites();

    void removeDeadExplosions();

    void prepareShapesByProgress(ExplosionState const& explosionState) const;
    void prepareSmokeShapesByProgress(ExplosionState const& explosionState) const;

    std::vector<ExplosionState> m_explosions;

    std::shared_ptr<jt::Shape> m_shapeFire;
    std::vector<std::shared_ptr<jt::Shape>> m_shapesSmoke;
    std::shared_ptr<jt::Animation> m_spriteShockwave;
};

#endif // EXPLOSION_GUARD_HPP
