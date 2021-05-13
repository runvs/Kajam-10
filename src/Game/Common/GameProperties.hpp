#ifndef GAMEPROPERTIES_GUARD_HPP
#define GAMEPROPERTIES_GUARD_HPP

#include "Vector.hpp"

namespace Game {
struct GameProperties {
    static float playerMovementSpeed() { return 100.0f; }
    static float playerMaxAllowedPredictionError() { return 4.0f * 4.0f; }
    static float shotVelocity() { return 200.0f; }
    static float shotLifeTime() { return 5.0f; }
    static float playerShootCooldown() { return 0.3f; }
    static int playerSizeInPixel() { return 16; }

    static jt::Vector2 GetWindowSize() { return jt::Vector2 { 800, 600 }; }
    static float GetZoom() { return 2.0f; }
    static jt::Vector2 GetScreenSize() { return GetWindowSize() * (1.0f / GetZoom()); }
};
} // namespace Game

#endif
