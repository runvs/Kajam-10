#ifndef GAMEPROPERTIES_GUARD_HPP
#define GAMEPROPERTIES_GUARD_HPP

#include "PlayerState.hpp"
#include "Vector.hpp"

namespace Game {
struct GameProperties {
    static float playerMovementSpeed() { return 100.0f; }
    static float playerMaxAllowedPredictionError() { return 4.0f * 4.0f; }
    static float playerShootCooldown() { return 0.1f; }
    static int playerSizeInPixel() { return 16; }
    static int playerMaxHealth() { return 12; }
    static float playerRespawnTime() { return 5.0f; }

    static float shotVelocity() { return 200.0f; }
    static float shotLifeTime() { return 5.0f; }
    static float shotBaseJitterAmount() { return 0.1f; }
    static float shotJitterSigma() { return 0.25f; }
    static jt::Vector2 shotHalfSize() { return jt::Vector2 { 4, 4 }; }
    static jt::Vector2 shotSize() { return 2.0f * shotHalfSize(); }

    static int enemyDefaultHealth() { return 3; }
    static jt::Vector2 enemyHalfSize() { return jt::Vector2 { 8, 8 }; }
    static int spawnerEnemiesPerPlayer() { return 6; }

    static jt::Vector2 displayWindowSize() { return jt::Vector2 { 800, 600 }; }
    static float displayZoom() { return 2.0f; }
    static jt::Vector2 displayScreenSize() { return displayWindowSize() * (1.0f / displayZoom()); }

    static float scrollSpeed() { return 25.0f; }
};
} // namespace Game

#endif
