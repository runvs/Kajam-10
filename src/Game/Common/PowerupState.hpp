#ifndef POWERUP_GUARD_HPP
#define POWERUP_GUARD_HPP
#include "Vector.hpp"

#include <SFML/Network/Packet.hpp>

struct PowerupState {
    jt::Vector2 position { 0.0f, 0.0f };
    int type { 0 };
    bool _isAlive { true };
};

sf::Packet& operator<<(sf::Packet& packet, PowerupState& powerupState);
sf::Packet& operator<<(sf::Packet& packet, PowerupState const& powerupState);
sf::Packet& operator>>(sf::Packet& packet, PowerupState& powerupState);

void updatePowerupState(PowerupState& powerup, float elapsed);

#endif
