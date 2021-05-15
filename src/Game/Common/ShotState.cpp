#include "ShotState.hpp"
#include "Conversions.hpp"
#include "GameProperties.hpp"
#include "MathHelper.hpp"
#include "Random.hpp"
#include <cstdlib>

sf::Packet& operator<<(sf::Packet& packet, ShotState& shotState)
{
    return packet << shotState.position << shotState.direction << shotState.fromPlayer;
}

sf::Packet& operator<<(sf::Packet& packet, ShotState const& shotState)
{
    return packet << shotState.position << shotState.direction << shotState.fromPlayer;
}

sf::Packet& operator>>(sf::Packet& packet, ShotState& shotState)
{
    return packet >> shotState.position >> shotState.direction >> shotState.fromPlayer;
}

void updateShotState(ShotState& s, float elapsed)
{
    s.position += s.direction * elapsed * Game::GameProperties::shotVelocity();
    s._age += elapsed;
}

jt::Vector2 getShotJitterDirection(float jitterAmount)
{
    jt::Vector2 const& vec { 0, -1 };
    return getShotJitterDirection(jitterAmount, vec);
}

/**
 * jitterAmount 1.0 is 180 degrees
 */
jt::Vector2 getShotJitterDirection(float jitterAmount, jt::Vector2 const& baseVector)
{
    auto const sigma = jitterAmount * Game::GameProperties::shotJitterSigma();
    auto const randomAngle = 90.0f * jt::Random::getFloatGauss(0, sigma);
    return jt::MathHelper::rotateBy(baseVector, randomAngle);
}