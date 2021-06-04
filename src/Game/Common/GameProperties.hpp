#ifndef GAMEPROPERTIES_GUARD_HPP
#define GAMEPROPERTIES_GUARD_HPP

#include "Vector.hpp"

namespace Game {
struct GameProperties {
    static float playerMovementSpeed();
    static float playerMaxAllowedPredictionError();
    static float playerShootCooldown();
    static int playerSizeInPixel();
    static int playerMaxHealth();
    static float playerRespawnTime();
    static float playerGracePeriodAfterDeath();

    static float shotVelocity();
    static float shotLifeTime();
    static float shotBaseJitterAmount();
    static float shotJitterSigma();
    static jt::Vector2 shotHalfSizeCollision();
    static jt::Vector2 shotHalfSizeRotation();

    static int enemyDefaultHealth();
    static jt::Vector2 enemyHalfSize();
    static float enemyShotInterval();
    static int spawnerEnemiesPerPlayer();
    static int enemyKillsNeededForPowerup();

    static jt::Vector2 displayWindowSize();
    static float displayZoom();
    static jt::Vector2 displayScreenSize();
    static float hudBarMargin();
    static float hudBarWidth();

    static float scrollSpeed();

    static int scoreMax();
    static int scorePowerupBoostAmount();
    static int scoreEnemyKillBonus();
    static int scorePlayerDeathMalus();

    static jt::Vector2 playerHalfSize();

    static jt::Vector2 powerupHalfSize();

    static int powerupHealthAmount();

    static float explosionDuration();
    static jt::Vector2 explosionHalfSize();
    static jt::Vector2 explosionSize();
    static float enemyHealthIncrease();
    static int playerEnemyCollisionDamage();
    static float playerMovementSpeedSlowdownIfShooting();
};

} // namespace Game

#endif
