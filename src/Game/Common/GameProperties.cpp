#include "GameProperties.hpp"

namespace Game {

float GameProperties::playerMovementSpeed() { return 120.0f; }
float GameProperties::playerMovementSpeedSlowdownIfShooting() { return 0.55f; }
float GameProperties::playerMaxAllowedPredictionError() { return 4.0f * 4.0f; }
float GameProperties::playerShootCooldown() { return 0.1f; }
float GameProperties::playerSizeInPixel() { return 16; }
int GameProperties::playerMaxHealth() { return 12; }
float GameProperties::playerRespawnTime() { return 5.0f; }
float GameProperties::playerGracePeriodAfterDeath() { return 2.0f; }

float GameProperties::shotVelocity() { return 200.0f; }
float GameProperties::shotLifeTime() { return 5.0f; }
float GameProperties::shotBaseJitterAmount() { return 0.05f; }
float GameProperties::shotJitterSigma() { return 0.25f; }

jt::Vector2 GameProperties::shotHalfSizeCollision() { return jt::Vector2 { 4.0f, 8.0f }; }
jt::Vector2 GameProperties::shotHalfSizeRotation() { return jt::Vector2 { 8.0f, 4.0f }; }
jt::Vector2 GameProperties::playerHalfSize()
{
    return jt::Vector2 { (playerSizeInPixel()), (playerSizeInPixel()) } / 2.0f;
}
jt::Vector2 GameProperties::powerupHalfSize() { return playerHalfSize(); }
jt::Vector2 GameProperties::explosionHalfSize() { return explosionSize() / 2.0f; }
jt::Vector2 GameProperties::explosionSize() { return jt::Vector2 { 16.0f, 16.0f }; }
jt::Vector2 GameProperties::enemyHalfSize() { return jt::Vector2 { 8, 8 }; }

int GameProperties::enemyDefaultHealth() { return 4; }

int GameProperties::spawnerEnemiesPerPlayer() { return 5; }

int GameProperties::enemyKillsNeededForPowerup() { return 9; }
jt::Vector2 GameProperties::displayWindowSize() { return jt::Vector2 { 800, 600 }; }

float GameProperties::displayZoom() { return 2.0f; }

jt::Vector2 GameProperties::displayScreenSize()
{
    return displayWindowSize() * (1.0f / displayZoom());
}

float GameProperties::hudBarMargin() { return 4.0f; }
float GameProperties::hudBarWidth() { return 8.0f; }
float GameProperties::scrollSpeed() { return 28.0f; }

int GameProperties::scoreMax() { return 700; }
int GameProperties::scorePowerupBoostAmount() { return 80; }
int GameProperties::scoreEnemyKillBonus() { return 1; }
int GameProperties::scorePlayerDeathMalus() { return 60; }

int GameProperties::powerupHealthAmount() { return 4; }

float GameProperties::explosionDuration() { return 2.0f; }

float GameProperties::enemyHealthIncreasePerLevel() { return 4.0f; }
float GameProperties::enemyShotInterval() { return 5.5f; }

int GameProperties::playerEnemyCollisionDamage() { return 4; }

} // namespace Game
