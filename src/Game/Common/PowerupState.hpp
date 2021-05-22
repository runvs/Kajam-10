#ifndef POWERUP_GUARD_HPP
#define POWERUP_GUARD_HPP
#include "Vector.hpp"

#include <SFML/Network/Packet.hpp>

enum class PowerupType : int { POWERUP_HEALTH, POWERUP_SHOT, POWERUP_POINTS, POWERUP_MAXNUMBER };

struct PowerupState {
    jt::Vector2 position { 0.0f, 0.0f };
    int type { 0 };
    bool _alive { true };
};

sf::Packet& operator<<(sf::Packet& packet, PowerupState& powerupState);
sf::Packet& operator<<(sf::Packet& packet, PowerupState const& powerupState);
sf::Packet& operator>>(sf::Packet& packet, PowerupState& powerupState);

void updatePowerupState(PowerupState& powerup, float elapsed);

#endif
